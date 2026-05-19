#include "progressive/user_mask.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

namespace progressive {

std::string UserMaskRegistry::normalizeMxid(const std::string& mxid) {
    std::string r = mxid;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool isValidMxid(const std::string& mxid) {
    // @user:server or @user:server:port
    std::regex mxidRe(R"(@[a-zA-Z0-9._=\-/]+:[a-zA-Z0-9.\-]+(?::\d+)?)");
    return std::regex_match(mxid, mxidRe);
}

void UserMaskRegistry::setMask(const UserMask& mask) {
    masks_[normalizeMxid(mask.originalMxid)] = mask;
}

void UserMaskRegistry::removeMask(const std::string& mxid) {
    masks_.erase(normalizeMxid(mxid));
}

const UserMask* UserMaskRegistry::getMask(const std::string& mxid) const {
    auto it = masks_.find(normalizeMxid(mxid));
    if (it != masks_.end()) return &it->second;
    return nullptr;
}

bool UserMaskRegistry::isMasked(const std::string& mxid) const {
    return masks_.find(normalizeMxid(mxid)) != masks_.end();
}

std::string UserMaskRegistry::exportJson() const {
    auto escape = [](const std::string& s) -> std::string {
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
    for (const auto& [_, mask] : masks_) {
        if (i++ > 0) json << ",";
        json << "{";
        json << R"("originalMxid": ")" << escape(mask.originalMxid) << R"(",)";
        json << R"("displayName": ")" << escape(mask.displayName) << R"(",)";
        json << R"("avatarUrl": ")" << escape(mask.avatarUrl) << R"(")";
        if (!mask.overrideMxid.empty()) {
            json << R"(,"overrideMxid": ")" << escape(mask.overrideMxid) << R"(")";
        }
        json << "}";
    }
    json << "]";
    return json.str();
}

void UserMaskRegistry::importJson(const std::string& json) {
    masks_.clear();
    // Simple JSON array parser for UserMask objects
    size_t pos = 0;
    while (pos < json.size()) {
        // Find next {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;

        // Find matching }
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

        UserMask mask;
        mask.originalMxid = progressive::parseJsonStringValue(objStr, "originalMxid");
        mask.displayName  = progressive::parseJsonStringValue(objStr, "displayName");
        mask.avatarUrl    = progressive::parseJsonStringValue(objStr, "avatarUrl");
        mask.overrideMxid = progressive::parseJsonStringValue(objStr, "overrideMxid");

        if (!mask.originalMxid.empty()) {
            setMask(mask);
        }

        pos = end + 1;
    }
}

void UserMaskRegistry::clear() {
    masks_.clear();
}

std::string resolveDisplayName(const std::string& mxid, const std::string& originalName,
                               const UserMaskRegistry& registry) {
    auto mask = registry.getMask(mxid);
    if (mask && !mask->displayName.empty()) return mask->displayName;
    return originalName;
}

std::string resolveAvatarUrl(const std::string& mxid, const std::string& originalUrl,
                             const UserMaskRegistry& registry) {
    auto mask = registry.getMask(mxid);
    if (mask && !mask->avatarUrl.empty()) return mask->avatarUrl;
    return originalUrl;
}

} // namespace progressive
