#include "progressive/live_draft.hpp"

namespace progressive {

std::string liveDraftConfigToJson(const LiveDraftConfig& config) {
    return "{\"enabled\":" + std::string(config.enabled ? "true" : "false") +
           ",\"characterThreshold\":" + std::to_string(config.characterThreshold) +
           ",\"updateIntervalMs\":" + std::to_string(config.updateIntervalMs) +
           ",\"draftPrefix\":\"" + config.draftPrefix + "\""
           ",\"finalEditRemovesPrefix\":" + std::string(config.finalEditRemovesPrefix ? "true" : "false") + "}";
}

LiveDraftConfig liveDraftConfigFromJson(const std::string& json) {
    LiveDraftConfig c;
    auto pos = json.find("\"enabled\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            while (++pos < json.size() && (json[pos] == ' ' || json[pos] == '\t'));
            c.enabled = json.compare(pos, 4, "true") == 0;
        }
    }
    pos = json.find("\"characterThreshold\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            while (++pos < json.size() && (json[pos] == ' ' || json[pos] == '\t'));
            int val = 0;
            while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
                val = val * 10 + (json[pos] - '0'); pos++;
            }
            if (val > 0) c.characterThreshold = val;
        }
    }
    pos = json.find("\"updateIntervalMs\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            while (++pos < json.size() && (json[pos] == ' ' || json[pos] == '\t'));
            int val = 0;
            while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
                val = val * 10 + (json[pos] - '0'); pos++;
            }
            if (val > 0) c.updateIntervalMs = val;
        }
    }
    pos = json.find("\"draftPrefix\"");
    if (pos != std::string::npos) {
        pos = json.find('"', pos);
        if (pos != std::string::npos) {
            pos = json.find(':', pos);
            if (pos != std::string::npos) {
                pos = json.find('"', pos + 1);
                if (pos != std::string::npos) {
                    size_t end = ++pos;
                    while (end < json.size() && json[end] != '"') {
                        if (json[end] == '\\') end++;
                        end++;
                    }
                    c.draftPrefix = json.substr(pos, end - pos);
                }
            }
        }
    }
    pos = json.find("\"finalEditRemovesPrefix\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            while (++pos < json.size() && (json[pos] == ' ' || json[pos] == '\t'));
            c.finalEditRemovesPrefix = json.compare(pos, 4, "true") == 0;
        }
    }
    return c;
}

} // namespace progressive
