#include "progressive/room_mirror.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <regex>
#include <algorithm>

namespace progressive {

void RoomMirrorManager::addMirror(const MirrorConfig& config) {
    mirrors_[config.sourceRoomId] = config;
}

void RoomMirrorManager::removeMirror(const std::string& sourceRoomId) {
    mirrors_.erase(sourceRoomId);
}

std::vector<const MirrorConfig*> RoomMirrorManager::getMirrorsForSource(const std::string& sourceRoomId) const {
    std::vector<const MirrorConfig*> result;
    auto it = mirrors_.find(sourceRoomId);
    if (it != mirrors_.end() && it->second.enabled) {
        result.push_back(&it->second);
    }
    return result;
}

std::vector<const MirrorConfig*> RoomMirrorManager::getMirrorsForTarget(const std::string& mirrorRoomId) const {
    std::vector<const MirrorConfig*> result;
    for (const auto& [_, cfg] : mirrors_) {
        if (cfg.mirrorRoomId == mirrorRoomId && cfg.enabled) {
            result.push_back(&cfg);
        }
    }
    return result;
}

void RoomMirrorManager::setEnabled(const std::string& sourceRoomId, bool enabled) {
    auto it = mirrors_.find(sourceRoomId);
    if (it != mirrors_.end()) {
        it->second.enabled = enabled;
    }
}

bool RoomMirrorManager::isMirroring(const std::string& sourceRoomId) const {
    auto it = mirrors_.find(sourceRoomId);
    return it != mirrors_.end() && it->second.enabled;
}

std::string RoomMirrorManager::formatMirrorMessage(const MirrorMessage& msg) {
    std::ostringstream out;
    out << msg.senderName << " wrote in " << msg.sourceRoomName << ": " << msg.body;
    return out.str();
}

std::string RoomMirrorManager::generateDollMxid(const std::string& originalMxid, const std::string& targetServer) {
    auto username = extractUsername(originalMxid);
    if (username.empty()) return {};

    // Generate doll name: alice → alicedoll
    std::string dollName = username + "doll";

    // Ensure it matches MXID format
    if (dollName.size() > 255) dollName = dollName.substr(0, 255);

    return "@" + dollName + ":" + targetServer;
}

bool RoomMirrorManager::isValidDollMxid(const std::string& mxid) {
    std::regex mxidRe(R"(@[a-zA-Z0-9._=\-/]+:[a-zA-Z0-9.\-]+(?::\d+)?)");
    if (!std::regex_match(mxid, mxidRe)) return false;

    auto username = extractUsername(mxid);
    // Must contain "doll" suffix
    return username.size() > 4 && username.rfind("doll") == username.size() - 4;
}

std::string RoomMirrorManager::extractUsername(const std::string& mxid) {
    if (mxid.empty() || mxid[0] != '@') return {};
    auto colonPos = mxid.find(':');
    if (colonPos == std::string::npos) return {};
    return mxid.substr(1, colonPos - 1);
}

DollAccount RoomMirrorManager::prepareDollRegistration(
    const std::string& originalMxid,
    const std::string& targetHomeserver,
    const std::string& adminToken
) {
    DollAccount doll;
    doll.originalMxid = originalMxid;
    doll.homeserverUrl = targetHomeserver;
    doll.dollMxid = generateDollMxid(originalMxid, targetHomeserver);
    return doll;
}

std::string RoomMirrorManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "[";
    size_t i = 0;
    for (const auto& [_, cfg] : mirrors_) {
        if (i++ > 0) json << ",";
        json << "{";
        json << R"("sourceRoomId": ")" << esc(cfg.sourceRoomId) << R"(",)";
        json << R"("sourceRoomName": ")" << esc(cfg.sourceRoomName) << R"(",)";
        json << R"("mirrorRoomId": ")" << esc(cfg.mirrorRoomId) << R"(",)";
        json << R"("mirrorRoomName": ")" << esc(cfg.mirrorRoomName) << R"(",)";
        json << R"("enabled": )" << (cfg.enabled ? "true" : "false") << ",";
        json << R"("useDolls": )" << (cfg.useDolls ? "true" : "false");
        json << "}";
    }
    json << "]";
    return json.str();
}

void RoomMirrorManager::importJson(const std::string& json) {
    mirrors_.clear();
    size_t pos = 0;
    while (true) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;

        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') --depth;
            if (depth == 0) break;
            ++end;
        }
        if (end >= json.size()) break;

        std::string obj = json.substr(pos, end - pos + 1);
        MirrorConfig cfg;
        cfg.sourceRoomId   = progressive::parseJsonStringValue(obj, "sourceRoomId");
        cfg.sourceRoomName = progressive::parseJsonStringValue(obj, "sourceRoomName");
        cfg.mirrorRoomId   = progressive::parseJsonStringValue(obj, "mirrorRoomId");
        cfg.mirrorRoomName = progressive::parseJsonStringValue(obj, "mirrorRoomName");
        auto enabledStr    = progressive::parseJsonStringValue(obj, "enabled");
        auto dollsStr      = progressive::parseJsonStringValue(obj, "useDolls");
        cfg.enabled  = (enabledStr == "true");
        cfg.useDolls = (dollsStr == "true");

        if (!cfg.sourceRoomId.empty()) mirrors_[cfg.sourceRoomId] = cfg;

        pos = end + 1;
    }
}

void RoomMirrorManager::clear() {
    mirrors_.clear();
}

HomeserverCapability parseRegistrationCapabilities(const std::string& wellKnownJson) {
    HomeserverCapability caps;

    // Parse "m.registration_disabled" or similar fields
    auto regDisabled = progressive::parseJsonStringValue(wellKnownJson, "m.registration_disabled");
    caps.registrationEnabled = (regDisabled != "true");

    // Check flows for email/captcha requirements
    // Simple heuristic: look for "m.login.email.identity" or "recaptcha" in the JSON
    auto lower = wellKnownJson;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    caps.emailRequired = lower.find("email") != std::string::npos;
    caps.captchaRequired = lower.find("captcha") != std::string::npos ||
                           lower.find("recaptcha") != std::string::npos;

    return caps;
}

} // namespace progressive
