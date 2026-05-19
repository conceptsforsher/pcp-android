#include "progressive/invitation_hide.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

void InvitationHideList::hide(const HiddenInvitation& invite) {
    HiddenInvitation copy = invite;
    if (copy.hiddenAt == 0) {
        copy.hiddenAt = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    hidden_[invite.roomId] = copy;
}

void InvitationHideList::unhide(const std::string& roomId) {
    hidden_.erase(roomId);
}

bool InvitationHideList::isHidden(const std::string& roomId) const {
    return hidden_.find(roomId) != hidden_.end();
}

std::vector<HiddenInvitation> InvitationHideList::getAll() const {
    std::vector<HiddenInvitation> result;
    for (const auto& p : hidden_) {
        const auto& inv = p.second;
        result.push_back(inv);
    }
    return result;
}

std::string InvitationHideList::exportJson() const {
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
    size_t i = 0;
    for (const auto& p : hidden_) {
        const auto& inv = p.second;
        if (i++ > 0) json << ",";
        json << "{";
        json << R"("roomId": ")" << esc(inv.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(inv.roomName) << R"(",)";
        json << R"("inviterName": ")" << esc(inv.inviterName) << R"(",)";
        json << R"("inviterMxid": ")" << esc(inv.inviterMxid) << R"(",)";
        json << R"("hiddenAt": )" << inv.hiddenAt;
        json << "}";
    }
    json << "]";
    return json.str();
}

void InvitationHideList::importJson(const std::string& json) {
    hidden_.clear();
    size_t pos = 0;
    while (pos < json.size()) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;

        int depth = 0;
        auto start = pos;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') --depth;
            if (depth == 0) break;
            ++end;
        }
        if (end >= json.size()) break;

        std::string objStr = json.substr(start, end - start + 1);

        HiddenInvitation inv;
        inv.roomId      = progressive::parseJsonStringValue(objStr, "roomId");
        inv.roomName    = progressive::parseJsonStringValue(objStr, "roomName");
        inv.inviterName = progressive::parseJsonStringValue(objStr, "inviterName");
        inv.inviterMxid = progressive::parseJsonStringValue(objStr, "inviterMxid");
        auto hiddenAtStr = progressive::parseJsonStringValue(objStr, "hiddenAt");
        if (!hiddenAtStr.empty()) inv.hiddenAt = std::stoll(hiddenAtStr);

        if (!inv.roomId.empty()) {
            hidden_[inv.roomId] = inv;
        }

        pos = end + 1;
    }
}

void InvitationHideList::clear() {
    hidden_.clear();
}

} // namespace progressive
