#include "pcp/notif_mode.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {

NightModeManager::NightModeManager() {}

void NightModeManager::setMode(NightMode m) { cfg_.mode = m; }
NightMode NightModeManager::getMode() const { return cfg_.mode; }

void NightModeManager::setNightKeywords(const std::vector<std::string>& keywords) {
    cfg_.nightKeywords = keywords;
}
void NightModeManager::addNightKeyword(const std::string& kw) {
    if (!kw.empty() && std::find(cfg_.nightKeywords.begin(), cfg_.nightKeywords.end(), kw) == cfg_.nightKeywords.end())
        cfg_.nightKeywords.push_back(kw);
}
void NightModeManager::removeNightKeyword(const std::string& kw) {
    cfg_.nightKeywords.erase(std::remove(cfg_.nightKeywords.begin(), cfg_.nightKeywords.end(), kw), cfg_.nightKeywords.end());
}

bool NightModeManager::matchKeyword(const std::string& body) const {
    if (cfg_.nightKeywords.empty()) return false;
    std::string lower;
    std::transform(body.begin(), body.end(), std::back_inserter(lower), ::tolower);
    for (const auto& kw : cfg_.nightKeywords) {
        std::string kwLower;
        std::transform(kw.begin(), kw.end(), std::back_inserter(kwLower), ::tolower);
        if (lower.find(kwLower) != std::string::npos) return true;
    }
    return false;
}

bool NightModeManager::shouldNotify(const std::string& body, const std::string& senderMxid,
                                     bool isRoomPing, bool isAlarm) const {
    if (cfg_.mode == NightMode::NORMAL) return true;
    if (isAlarm) return true;
    if (isRoomPing && cfg_.nightPingRooms) return true;
    if (matchKeyword(body)) return true;
    if (!senderMxid.empty() && std::find(cfg_.nightMxids.begin(), cfg_.nightMxids.end(), senderMxid) != cfg_.nightMxids.end())
        return true;
    return false;
}

std::string NightModeManager::toJson() const {
    std::ostringstream os;
    os << "{";
    os << "\"mode\":" << static_cast<int>(cfg_.mode);
    os << ",\"nightPingRooms\":" << (cfg_.nightPingRooms ? "true" : "false");
    os << ",\"nightKeywords\":[";
    for (size_t i = 0; i < cfg_.nightKeywords.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << cfg_.nightKeywords[i] << "\"";
    }
    os << "],\"nightMxids\":[";
    for (size_t i = 0; i < cfg_.nightMxids.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << cfg_.nightMxids[i] << "\"";
    }
    os << "]}";
    return os.str();
}

void NightModeManager::fromJson(const std::string& json) {
    cfg_ = NightNotifConfig{};
    if (json.find("\"mode\":1") != std::string::npos) cfg_.mode = NightMode::NIGHT;
    cfg_.nightPingRooms = json.find("\"nightPingRooms\":true") != std::string::npos;
    // Parse keywords array
    auto kwPos = json.find("\"nightKeywords\"");
    if (kwPos != std::string::npos) {
        auto start = json.find('[', kwPos);
        auto end = json.find(']', start);
        if (start != std::string::npos && end != std::string::npos) {
            std::string arr = json.substr(start + 1, end - start - 1);
            size_t s = 0;
            while (s < arr.size()) {
                auto q1 = arr.find('"', s);
                if (q1 == std::string::npos) break;
                auto q2 = arr.find('"', q1 + 1);
                if (q2 == std::string::npos) break;
                cfg_.nightKeywords.push_back(arr.substr(q1 + 1, q2 - q1 - 1));
                s = q2 + 1;
            }
        }
    }
    // Parse mxids array
    auto mxPos = json.find("\"nightMxids\"");
    if (mxPos != std::string::npos) {
        auto start = json.find('[', mxPos);
        auto end = json.find(']', start);
        if (start != std::string::npos && end != std::string::npos) {
            std::string arr = json.substr(start + 1, end - start - 1);
            size_t s = 0;
            while (s < arr.size()) {
                auto q1 = arr.find('"', s);
                if (q1 == std::string::npos) break;
                auto q2 = arr.find('"', q1 + 1);
                if (q2 == std::string::npos) break;
                cfg_.nightMxids.push_back(arr.substr(q1 + 1, q2 - q1 - 1));
                s = q2 + 1;
            }
        }
    }
}

} // namespace pcp
