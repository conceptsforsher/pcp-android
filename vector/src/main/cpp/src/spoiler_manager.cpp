#include "progressive/spoiler_manager.hpp"
#include <sstream>

namespace progressive {

// ====== Constructor ======

SpoilerManager::SpoilerManager() {}
void SpoilerManager::setConfig(const SpoilerConfig& config) { config_ = config; }
SpoilerConfig SpoilerManager::getConfig() const { return config_; }

// ====== Helpers ======

std::string SpoilerManager::buildSpoilerSpan(const std::string& innerContent, const std::string& reason) const {
    std::ostringstream os;
    os << "<span data-mx-spoiler";
    if (!reason.empty()) os << " data-mx-spoiler-reason=\"" << reason << "\"";
    if (!config_.spoilerLabel.empty()) os << " aria-label=\"" << config_.spoilerLabel << "\"";
    os << ">" << innerContent << "</span>";
    return os.str();
}

std::string SpoilerManager::buildImageTag(const std::string& mxcUrl, const std::string& mimeType,
                                            int w, int h, int64_t size, const std::string& plainBody) const {
    std::ostringstream os;
    os << "<img src=\"" << mxcUrl << "\"";
    if (!mimeType.empty()) os << " data-mime-type=\"" << mimeType << "\"";
    if (w > 0) os << " width=\"" << w << "\"";
    if (h > 0) os << " height=\"" << h << "\"";
    if (size > 0) os << " data-size=\"" << size << "\"";
    os << " alt=\"" << plainBody << "\"";
    os << " title=\"" << plainBody << "\"";
    os << " />";
    return os.str();
}

std::string SpoilerManager::buildVideoTag(const std::string& mxcUrl, const std::string& mimeType,
                                            int durationMs, const std::string& plainBody) const {
    std::ostringstream os;
    os << "<video src=\"" << mxcUrl << "\"";
    if (!mimeType.empty()) os << " data-mime-type=\"" << mimeType << "\"";
    if (durationMs > 0) os << " data-duration=\"" << durationMs << "\"";
    os << " alt=\"" << plainBody << "\"";
    os << " title=\"" << plainBody << "\"";
    os << " controls />";
    return os.str();
}

std::string SpoilerManager::buildFileTag(const std::string& mxcUrl, const std::string& mimeType,
                                           int64_t size, const std::string& plainBody) const {
    std::ostringstream os;
    os << "<a href=\"" << mxcUrl << "\"";
    if (!mimeType.empty()) os << " data-mime-type=\"" << mimeType << "\"";
    if (size > 0) os << " data-size=\"" << size << "\"";
    os << ">" << plainBody << "</a>";
    return os.str();
}

// ====== Build Spoiler Content ======

SpoilerContent SpoilerManager::buildImageSpoiler(const std::string& plainBody,
                                                   const std::string& mxcUrl,
                                                   const std::string& mimeType,
                                                   int width, int height,
                                                   int64_t sizeBytes,
                                                   const std::string& reason) {
    SpoilerContent sc;
    sc.plainBody = plainBody;
    sc.spoilerType = "image";
    sc.hasSpoiler = true;
    sc.reason = reason.empty() ? config_.spoilerReason : reason;

    std::string imgTag = buildImageTag(mxcUrl, mimeType, width, height, sizeBytes, plainBody);
    sc.formattedBody = buildSpoilerSpan(imgTag, sc.reason);

    return sc;
}

SpoilerContent SpoilerManager::buildTextSpoiler(const std::string& plainBody, const std::string& reason) {
    SpoilerContent sc;
    sc.plainBody = plainBody;
    sc.spoilerType = "text";
    sc.hasSpoiler = true;
    sc.reason = reason.empty() ? config_.spoilerReason : reason;

    // Text spoilers: plain text uses ||spoiler|| convention, formatted uses span
    sc.plainBody = "||" + plainBody + "||";
    sc.formattedBody = buildSpoilerSpan(plainBody, sc.reason);

    return sc;
}

SpoilerContent SpoilerManager::buildVideoSpoiler(const std::string& plainBody,
                                                   const std::string& mxcUrl,
                                                   const std::string& mimeType,
                                                   int durationMs,
                                                   const std::string& reason) {
    SpoilerContent sc;
    sc.plainBody = "[SPOILER: video] " + plainBody;
    sc.spoilerType = "video";
    sc.hasSpoiler = true;
    sc.reason = reason.empty() ? config_.spoilerReason : reason;

    std::string videoTag = buildVideoTag(mxcUrl, mimeType, durationMs, plainBody);
    sc.formattedBody = buildSpoilerSpan(videoTag, sc.reason);

    return sc;
}

SpoilerContent SpoilerManager::buildFileSpoiler(const std::string& plainBody,
                                                  const std::string& mxcUrl,
                                                  const std::string& mimeType,
                                                  int64_t sizeBytes,
                                                  const std::string& reason) {
    SpoilerContent sc;
    sc.plainBody = "[SPOILER: file] " + plainBody;
    sc.spoilerType = "file";
    sc.hasSpoiler = true;
    sc.reason = reason.empty() ? config_.spoilerReason : reason;

    std::string fileTag = buildFileTag(mxcUrl, mimeType, sizeBytes, plainBody);
    sc.formattedBody = buildSpoilerSpan(fileTag, sc.reason);

    return sc;
}

// ====== Detection ======

bool SpoilerManager::hasSpoiler(const std::string& formattedBody) const {
    return formattedBody.find("data-mx-spoiler") != std::string::npos;
}

std::string SpoilerManager::detectSpoilerType(const std::string& formattedBody) const {
    if (formattedBody.find("<img") != std::string::npos) return "image";
    if (formattedBody.find("<video") != std::string::npos) return "video";
    if (formattedBody.find("<a href") != std::string::npos) return "file";
    return "text";
}

std::string SpoilerManager::extractSpoilerReason(const std::string& formattedBody) const {
    auto pos = formattedBody.find("data-mx-spoiler-reason=\"");
    if (pos == std::string::npos) return "";

    pos += 24; // Skip 'data-mx-spoiler-reason="'
    auto end = formattedBody.find('"', pos);
    if (end == std::string::npos) return "";

    return formattedBody.substr(pos, end - pos);
}

// ====== Display Formatting ======

std::string SpoilerManager::formatSpoilerNotification(const std::string& senderName,
                                                       const std::string& spoilerType) const {
    return senderName + " sent a spoiler: " + spoilerType;
}

std::string SpoilerManager::formatSpoilerPreview(const std::string& spoilerType) const {
    if (spoilerType == "image") return "[SPOILER: image]";
    if (spoilerType == "video") return "[SPOILER: video]";
    if (spoilerType == "file") return "[SPOILER: file]";
    return "[SPOILER]";
}

// ====== Build Content ======

std::string SpoilerManager::buildSpoilerMessageContent(const SpoilerContent& spoiler,
                                                         const std::string& mxcUrl,
                                                         const std::string& msgType) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\""; else if (c == '\n') out += "\\n"; else out += c;
        }
        return out;
    };

    std::ostringstream os;
    os << R"({"msgtype":")" << msgType << R"(")";
    os << R"(,"body":")" << esc(spoiler.plainBody) << R"(")";
    os << R"(,"url":")" << mxcUrl << R"(")";
    os << R"(,"formatted_body":")" << esc(spoiler.formattedBody) << R"(")";
    os << R"(,"format":"org.matrix.custom.html")";
    os << R"(,"m.spoiler":true)";
    if (!spoiler.reason.empty()) os << R"(,"m.spoiler_reason":")" << esc(spoiler.reason) << R"(")";
    os << "}";
    return os.str();
}

// ====== Serialization ======

std::string SpoilerManager::spoilerToJson(const SpoilerContent& spoiler) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << spoiler.spoilerType
       << R"(","has_spoiler":)" << (spoiler.hasSpoiler ? "true" : "false")
       << R"(,"body":")" << esc(spoiler.plainBody)
       << R"(","reason":")" << esc(spoiler.reason) << R"(")";
    os << "}";
    return os.str();
}

std::string SpoilerManager::configToJson() const {
    std::ostringstream os;
    os << R"({"show_checkbox":)" << (config_.showSpoilerCheckbox ? "true" : "false")
       << R"(,"auto_nsfw":)" << (config_.autoSpoilerNsfw ? "true" : "false")
       << R"(,"reveal_default":)" << (config_.revealByDefault ? "true" : "false")
       << R"(,"label":")" << config_.spoilerLabel << R"(")"
       << "}";
    return os.str();
}

} // namespace progressive
