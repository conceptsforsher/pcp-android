#include "progressive/media_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <iomanip>

namespace progressive {

std::string buildMediaUploadBody(const MediaUploadConfig& config) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    if (!config.fileName.empty())
        json << R"("filename": ")" << esc(config.fileName) << R"(",)";
    json << R"("content_type": ")" << esc(config.mimeType) << R"(")";
    json << "}";
    return json.str();
}

std::string parseUploadResponse(const std::string& responseJson) {
    return parseJsonStringValue(responseJson, "content_uri");
}

MediaDownloadInfo parseMediaDownloadInfo(const std::string& mxcUri, const std::string& responseJson) {
    MediaDownloadInfo info;
    info.mxcUri = mxcUri;
    info.mimeType  = parseJsonStringValue(responseJson, "content_type");
    info.fileName  = parseJsonStringValue(responseJson, "filename");

    auto size = parseJsonStringValue(responseJson, "size");
    if (!size.empty()) info.fileSize = std::stoll(size);

    info.isComplete = true;
    return info;
}

std::string buildThumbnailDimensions(const MediaUploadConfig& config) {
    std::ostringstream out;
    out << config.maxThumbnailW << "x" << config.maxThumbnailH;
    return out.str();
}

double computeUploadProgress(int64_t uploaded, int64_t total) {
    if (total <= 0) return 0.0;
    return static_cast<double>(uploaded) / total * 100.0;
}

std::string formatUploadProgress(int64_t uploaded, int64_t total) {
    std::ostringstream out;
    if (uploaded < 1024 && total < 1024) {
        out << uploaded << " / " << total << " B";
    } else {
        out << std::fixed << std::setprecision(1)
            << (uploaded / 1024.0) << " / " << (total / 1024.0) << " KB";
    }
    out << " (" << static_cast<int>(computeUploadProgress(uploaded, total)) << "%)";
    return out.str();
}

bool shouldGenerateThumbnail(const std::string& mimeType) {
    return mimeType.rfind("image/", 0) == 0 || mimeType.rfind("video/", 0) == 0;
}

std::string getMatrixContentType(const std::string& mimeType) {
    if (mimeType == "image/jpeg") return "image/jpeg";
    if (mimeType == "image/png") return "image/png";
    if (mimeType == "image/gif") return "image/gif";
    if (mimeType == "image/webp") return "image/webp";
    if (mimeType == "video/mp4") return "video/mp4";
    if (mimeType == "video/webm") return "video/webm";
    if (mimeType == "audio/ogg") return "audio/ogg";
    if (mimeType == "audio/mp3" || mimeType == "audio/mpeg") return "audio/mpeg";
    if (mimeType == "application/pdf") return "application/pdf";
    return "application/octet-stream";
}

std::string mimeToMsgType(const std::string& mimeType) {
    if (mimeType.rfind("image/", 0) == 0) return "m.image";
    if (mimeType.rfind("video/", 0) == 0) return "m.video";
    if (mimeType.rfind("audio/", 0) == 0) return "m.audio";
    return "m.file";
}

bool isValidBlurhash(const std::string& hash) {
    if (hash.empty() || hash.size() > 100) return false;
    // Valid blurhash chars
    for (char c : hash) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
              (c >= 'a' && c <= 'z') || c == '$' || c == '%' || c == '*' ||
              c == '+' || c == '-' || c == '.' || c == '/' || c == ':' ||
              c == ';' || c == '<' || c == '=' || c == '>' || c == '?' ||
              c == '@' || c == '[' || c == ']' || c == '^' || c == '_' ||
              c == '{' || c == '|' || c == '}' || c == '~')) {
            return false;
        }
    }
    return hash.size() >= 6;
}

BlurhashResult parseBlurhash(const std::string& contentJson) {
    BlurhashResult result;
    auto info = parseJsonStringValue(contentJson, "info");
    if (info.empty()) return result;

    std::string wrapped = "{" + info + "}";
    result.hash = parseJsonStringValue(wrapped, "blurhash");

    auto xStr = parseJsonStringValue(wrapped, "blurhash_x");
    auto yStr = parseJsonStringValue(wrapped, "blurhash_y");
    if (!xStr.empty()) result.componentsX = std::stoi(xStr);
    if (!yStr.empty()) result.componentsY = std::stoi(yStr);

    result.valid = isValidBlurhash(result.hash);
    return result;
}

} // namespace progressive
