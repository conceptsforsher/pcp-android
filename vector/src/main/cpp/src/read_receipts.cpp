#include "progressive/read_receipts.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace progressive {

void sortByTimestampDesc(std::vector<ReceiptEntry>& receipts) {
    std::sort(receipts.begin(), receipts.end(), [](const ReceiptEntry& a, const ReceiptEntry& b) {
        return a.timestamp > b.timestamp;
    });
}

std::vector<ReceiptEntry> deduplicateReceipts(
    const std::vector<ReceiptEntry>& receipts
) {
    std::unordered_map<std::string, ReceiptEntry> bestByUser;
    for (const auto& r : receipts) {
        auto it = bestByUser.find(r.userId);
        if (it == bestByUser.end() || r.timestamp > it->second.timestamp) {
            bestByUser[r.userId] = r;
        }
    }
    std::vector<ReceiptEntry> result;
    for (const auto& [_, entry] : bestByUser) {
        result.push_back(entry);
    }
    sortByTimestampDesc(result);
    return result;
}

ReceiptDisplay computeReceiptDisplay(
    const std::vector<ReceiptEntry>& allReceipts,
    int maxVisible
) {
    ReceiptDisplay display;
    display.maxVisible = maxVisible;

    if (allReceipts.empty()) return display;

    // Deduplicate and sort
    auto deduped = deduplicateReceipts(allReceipts);
    display.totalCount = static_cast<int>(deduped.size());

    // Take visible entries
    int visibleCount = std::min(static_cast<int>(deduped.size()), maxVisible);
    for (int i = 0; i < visibleCount; ++i) {
        display.visibleEntries.push_back(deduped[i]);
    }

    // Compute overflow
    display.overflowCount = display.totalCount - visibleCount;

    // Format accessibility text
    display.accessibilityText = formatReceiptAccessibility(
        display.visibleEntries, display.overflowCount
    );

    return display;
}

std::string formatOverflowLabel(int count) {
    if (count <= 0) return "";
    return "+" + std::to_string(count);
}

std::string formatReceiptAccessibility(
    const std::vector<ReceiptEntry>& visibleEntries,
    int overflowCount
) {
    std::ostringstream out;

    int visCount = static_cast<int>(visibleEntries.size());
    if (visCount == 0) return "";

    // Format names: "Alice, Bob, and Charlie read"
    for (int i = 0; i < visCount; ++i) {
        if (i > 0) {
            if (i == visCount - 1 && overflowCount == 0) {
                out << (visCount == 2 ? " and " : ", and ");
            } else {
                out << ", ";
            }
        }
        auto name = visibleEntries[i].displayName;
        if (name.empty()) name = visibleEntries[i].userId;
        out << name;
    }

    if (overflowCount > 0) {
        out << " and " << overflowCount << (overflowCount == 1 ? " other" : " others");
    }

    out << " read";
    return out.str();
}

std::string receiptDisplayToJson(const ReceiptDisplay& display) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("visibleEntries": [)";
    for (size_t i = 0; i < display.visibleEntries.size(); ++i) {
        if (i > 0) json << ",";
        const auto& e = display.visibleEntries[i];
        json << R"({"userId": ")" << esc(e.userId) << R"(")";
        json << R"(,"displayName": ")" << esc(e.displayName) << R"(")";
        json << R"(,"avatarUrl": ")" << esc(e.avatarUrl) << R"(")";
        json << R"(,"timestamp": )" << e.timestamp << "}";
    }
    json << "],";
    json << R"("overflowCount": )" << display.overflowCount << ",";
    json << R"("overflowLabel": ")" << formatOverflowLabel(display.overflowCount) << R"(",)";
    json << R"("totalCount": )" << display.totalCount << ",";
    json << R"("accessibilityText": ")" << esc(display.accessibilityText) << R"(",)";
    json << R"("maxVisible": )" << display.maxVisible;
    json << "}";
    return json.str();
}

} // namespace progressive
