#include "progressive/edit_history.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

EditHistory parseEditHistory(const std::string& originalEventId, const std::string& originalBody,
    const std::string& originalSenderId, const std::string& originalSenderName,
    int64_t originalSentAtMs, const std::string& annotationsJson) {
    // Original Kotlin (EventAnnotationsSummary.kt → EditAggregatedSummary):
    //   val editSummary: EditAggregatedSummary? — contains list of EditionOfEvent
    //   Each EditionOfEvent has: eventId, timestamp, isLocalEcho, event: EventEntity?
    EditHistory history;
    history.originalEventId = originalEventId;
    history.originalBody = originalBody;
    history.originalSenderId = originalSenderId;
    history.originalSenderName = originalSenderName;
    history.originalSentAtMs = originalSentAtMs;

    // Parse editions array from annotations
    auto editSummary = parseJsonStringValue(annotationsJson, "editSummary");
    if (editSummary.empty()) return history;

    std::string wrapped = "{" + editSummary + "}";
    auto editions = parseJsonStringValue(wrapped, "editions");

    if (editions.empty()) return history;

    // Parse each edition object from the array
    // Original Kotlin iterates: editSummary.editions.forEach { edition -> ... }
    size_t pos = 0;
    while (true) {
        pos = editions.find("\"eventId\"", pos);
        if (pos == std::string::npos) break;

        auto objStart = editions.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < editions.size()) {
            if (editions[objEnd] == '{') ++depth;
            else if (editions[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= editions.size()) break;

        std::string obj = editions.substr(objStart, objEnd - objStart + 1);

        EditEntry entry;
        entry.editEventId = parseJsonStringValue(obj, "eventId");

        auto ts = parseJsonStringValue(obj, "timestamp");
        if (!ts.empty()) entry.editedAtMs = std::stoll(ts);

        auto echo = parseJsonStringValue(obj, "isLocalEcho");
        entry.isLocalEcho = (echo == "true");

        // Get the new body from the event content
        auto content = parseJsonStringValue(obj, "content");
        if (!content.empty()) {
            std::string cw = "{" + content + "}";
            entry.newBody = parseJsonStringValue(cw, "body");
            if (entry.newBody.empty()) {
                // Try m.new_content block
                auto newContent = parseJsonStringValue(cw, "m.new_content");
                if (!newContent.empty()) {
                    entry.newBody = parseJsonStringValue("{" + newContent + "}", "body");
                }
            }
        }

        if (!entry.editEventId.empty()) {
            history.edits.push_back(entry);
            history.editCount++;
        }

        pos = objEnd + 1;
    }

    // Sort newest first (as in original Kotlin)
    std::sort(history.edits.begin(), history.edits.end(),
        [](const EditEntry& a, const EditEntry& b) { return a.editedAtMs > b.editedAtMs; });

    history.hasPendingEdits = hasPendingEdits(history);

    return history;
}

std::string formatEditHistory(const EditHistory& history) {
    std::ostringstream out;
    out << "Edit History\n";
    out << "============\n";
    out << "Original by " << history.originalSenderName << "\n";
    out << history.originalBody << "\n\n";

    if (history.edits.empty()) {
        out << "No edits.\n";
        return out.str();
    }

    for (size_t i = 0; i < history.edits.size(); ++i) {
        out << formatEditEntry(history.edits[i], static_cast<int>(i + 1));
    }

    return out.str();
}

std::string formatEditEntry(const EditEntry& entry, int index) {
    std::ostringstream out;
    out << "Edit #" << index << " by " << entry.editorName;
    if (entry.isLocalEcho) out << " (pending)";
    out << "\n";
    out << (entry.newBody.size() > 100 ? entry.newBody.substr(0, 97) + "..." : entry.newBody);
    out << "\n\n";
    return out.str();
}

bool hasPendingEdits(const EditHistory& history) {
    for (const auto& edit : history.edits) {
        if (edit.isLocalEcho) return true;
    }
    return false;
}

std::string getCurrentBody(const EditHistory& history) {
    // Return the most recent edit body, or original if no edits
    if (history.edits.empty()) return history.originalBody;
    return history.edits[0].newBody;
}

std::string getEditBadgeText(int editCount) {
    if (editCount <= 0) return "";
    if (editCount == 1) return "(edited)";
    return "(edited " + std::to_string(editCount) + " times)";
}

std::string getEditCountBadge(int editCount) {
    if (editCount <= 0) return "";
    if (editCount == 1) return "(edited)";
    if (editCount == 2) return "(edited twice)";
    return "(edited " + std::to_string(editCount) + " times)";
}

std::string computeEditDiffSummary(const std::string& oldBody, const std::string& newBody) {
    if (oldBody.empty() && newBody.empty()) return "no change";

    int added = static_cast<int>(newBody.size()) - static_cast<int>(oldBody.size());
    int absDiff = std::abs(added);

    if (added > 0) {
        return "+" + std::to_string(added) + " chars";
    } else if (added < 0) {
        return std::to_string(added) + " chars";
    }
    return "no size change";
}

std::string formatEditSummary(const EditEntry& entry) {
    std::ostringstream out;

    // Original Kotlin (ViewEditHistoryBottomSheet): shows editor + time
    std::string editor = entry.editorName.empty() ? entry.editorId : entry.editorName;
    out << editor;

    if (entry.editedAtMs > 0) {
        time_t ts = static_cast<time_t>(entry.editedAtMs / 1000);
        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "%H:%M", localtime(&ts));
        out << " at " << timeBuf;
    }

    // Add diff summary
    if (!entry.previousBody.empty() && !entry.newBody.empty()) {
        out << " — " << computeEditDiffSummary(entry.previousBody, entry.newBody);
    }

    return out.str();
}

bool shouldCollapseEdits(const EditHistory& history, int threshold) {
    return history.editCount >= threshold;
}

std::string formatExpandedEditList(const EditHistory& history) {
    std::ostringstream out;
    for (size_t i = 0; i < history.edits.size(); ++i) {
        if (i > 0) out << "\n";
        out << (i + 1) << ". " << formatEditSummary(history.edits[i]);
    }
    return out.str();
}

} // namespace progressive
