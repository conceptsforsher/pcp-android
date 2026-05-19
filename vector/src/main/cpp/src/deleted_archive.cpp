#include "progressive/deleted_archive.hpp"
#include <sstream>
#include <chrono>
#include <algorithm>

namespace progressive {

void DeletedMessageArchive::archive(const DeletedEvent& event) {
    DeletedEvent copy = event;
    if (copy.deletedAt == 0) {
        copy.deletedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    entries_.push_back(copy);
}

std::vector<DeletedEvent> DeletedMessageArchive::getByRoom(const std::string& roomId) const {
    std::vector<DeletedEvent> result;
    for (const auto& e : entries_) {
        if (e.roomId == roomId) result.push_back(e);
    }
    return result;
}

std::vector<DeletedEvent> DeletedMessageArchive::getAll() const {
    return entries_;
}

void DeletedMessageArchive::purge(const std::string& eventId) {
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const DeletedEvent& e) { return e.eventId == eventId; }
    ), entries_.end());
}

void DeletedMessageArchive::clear() {
    entries_.clear();
}

std::string DeletedMessageArchive::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (i > 0) json << ",";
        const auto& e = entries_[i];
        json << "{";
        json << R"("eventId": ")" << esc(e.eventId) << R"(",)";
        json << R"("roomId": ")" << esc(e.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(e.roomName) << R"(",)";
        json << R"("senderName": ")" << esc(e.senderName) << R"(",)";
        json << R"("body": ")" << esc(e.body) << R"(",)";
        json << R"("msgType": ")" << esc(e.msgType) << R"(",)";
        json << R"("timestamp": ")" << esc(e.timestamp) << R"(",)";
        json << R"("deletedAt": )" << e.deletedAt << ",";
        json << R"("deletedBy": ")" << esc(e.deletedBy) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

} // namespace progressive
