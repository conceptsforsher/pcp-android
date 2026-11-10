#include "pcp/cross_signing_manager.hpp"
#include "pcp/device_manager_full.hpp"
#include "pcp/poll_manager.hpp"
#include "pcp/room_directory_manager.hpp"
#include "pcp/room_state_manager.hpp"
#include "pcp/server_notice_manager.hpp"
#include "pcp/space_graph.hpp"
#include <sstream>

namespace pcp {

// ---- content_utils free functions (real implementations) ----

std::string buildMxcUri(const std::string& serverName, const std::string& mediaId) {
    return "mxc://" + serverName + "/" + mediaId;
}

std::string ensureCorrectFormattedBodyInTextReply(const std::string& repliedEventBody,
                                                   const std::string& originalBody,
                                                   const std::string& replyFormattedBody) {
    if (repliedEventBody.empty()) return originalBody;
    std::string fallback = "> <" + repliedEventBody + ">\n\n";
    auto pos = replyFormattedBody.find("<mx-reply>");
    if (pos == std::string::npos) {
        pos = replyFormattedBody.find("<blockquote>");
    }
    if (pos != std::string::npos) {
        auto end = replyFormattedBody.find("</mx-reply>", pos);
        if (end == std::string::npos) end = replyFormattedBody.find("</blockquote>", pos);
        if (end != std::string::npos) {
            end = replyFormattedBody.find('>', end);
            if (end != std::string::npos) end++;
            return replyFormattedBody.substr(0, pos) + replyFormattedBody.substr(end) + fallback;
        }
    }
    return fallback + originalBody;
}

std::string extractUsefulTextFromReply(const std::string& repliedBody) {
    if (repliedBody.empty()) return "";
    std::string result;
    bool inTag = false;
    for (size_t i = 0; i < repliedBody.size(); i++) {
        if (repliedBody[i] == '<') {
            inTag = true;
            continue;
        }
        if (inTag) {
            if (repliedBody[i] == '>') inTag = false;
            continue;
        }
        result += repliedBody[i];
    }
    // Limit to ~150 chars
    if (result.size() > 150) result = result.substr(0, 147) + "...";
    return result;
}

std::string formatSpoilerTextFromHtml(const std::string& formattedBody) {
    std::string result;
    bool inTag = false;
    bool inSpoiler = false;
    for (size_t i = 0; i < formattedBody.size(); i++) {
        if (formattedBody[i] == '<') {
            inTag = true;
            // Check for spoiler tag
            if (i + 20 < formattedBody.size()) {
                auto sub = formattedBody.substr(i + 1, 20);
                if (sub.find("data-mx-spoiler") != std::string::npos) inSpoiler = true;
            }
            if (i + 8 < formattedBody.size()) {
                auto sub = formattedBody.substr(i, 8);
                if (sub == "</span>" && inSpoiler) {
                    result += " (spoiler)";
                    inSpoiler = false;
                }
            }
            continue;
        }
        if (inTag) {
            if (formattedBody[i] == '>') inTag = false;
            continue;
        }
        result += formattedBody[i];
    }
    return result;
}

std::string getEditedTargetEventId(const std::string& contentJson) {
    auto pos = contentJson.find("\"m.relates_to\"");
    if (pos == std::string::npos) return "";
    auto eventPos = contentJson.find("\"event_id\"", pos);
    if (eventPos == std::string::npos) return "";
    auto start = contentJson.find('"', eventPos + 11);
    if (start == std::string::npos) return "";
    auto end = contentJson.find('"', start + 1);
    if (end == std::string::npos) return "";
    return contentJson.substr(start + 1, end - start - 1);
}

std::string getExtensionFromMimeType(const std::string& mimetype) {
    if (mimetype == "image/jpeg") return ".jpg";
    if (mimetype == "image/png")  return ".png";
    if (mimetype == "image/gif")  return ".gif";
    if (mimetype == "image/webp") return ".webp";
    if (mimetype == "image/svg+xml") return ".svg";
    if (mimetype == "video/mp4")  return ".mp4";
    if (mimetype == "video/webm") return ".webm";
    if (mimetype == "audio/mp4")  return ".m4a";
    if (mimetype == "audio/mp3")  return ".mp3";
    if (mimetype == "audio/ogg")  return ".ogg";
    if (mimetype == "audio/wav")  return ".wav";
    if (mimetype == "audio/flac") return ".flac";
    if (mimetype == "text/plain") return ".txt";
    if (mimetype == "application/pdf") return ".pdf";
    if (mimetype == "application/zip") return ".zip";
    return "";
}

std::string getLatestEditEventId(const std::string& editSummaryJson, const std::string& originalEventId) {
    auto pos = editSummaryJson.find("\"latest_event_id\"");
    if (pos == std::string::npos) return originalEventId;
    auto start = editSummaryJson.find('"', pos + 18);
    if (start == std::string::npos) return originalEventId;
    auto end = editSummaryJson.find('"', start + 1);
    if (end == std::string::npos) return originalEventId;
    return editSummaryJson.substr(start + 1, end - start - 1);
}

bool hasTextWithImage(const std::string& contentJson) {
    auto bodyPos = contentJson.find("\"body\"");
    if (bodyPos == std::string::npos) return false;
    auto msgTypePos = contentJson.find("\"msgtype\"");
    if (msgTypePos == std::string::npos) return false;
    bool isImage = contentJson.find("\"m.image\"", msgTypePos) != std::string::npos ||
                   contentJson.find("\"m.image\"", msgTypePos + 20) != std::string::npos;
    if (!isImage) return false;
    auto start = contentJson.find('"', bodyPos + 7);
    if (start == std::string::npos) return false;
    auto end = contentJson.find('"', start + 1);
    if (end == std::string::npos) return false;
    std::string body = contentJson.substr(start + 1, end - start - 1);
    return !body.empty();
}

std::string normalizeMimeType(const std::string& mimeType) {
    if (mimeType.empty()) return mimeType;
    auto slash = mimeType.find('/');
    if (slash == std::string::npos) return mimeType;
    std::string result = mimeType;
    std::transform(result.begin() + slash + 1, result.end(), result.begin() + slash + 1, ::tolower);
    return result;
}

std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl,
                                     int width, int height, const std::string& method) {
    const std::string prefix = "mxc://";
    if (mxcUrl.compare(0, prefix.size(), prefix) != 0) return mxcUrl;
    auto slash = mxcUrl.find('/', prefix.size());
    if (slash == std::string::npos) return mxcUrl;
    auto server = mxcUrl.substr(prefix.size(), slash - prefix.size());
    auto mediaId = mxcUrl.substr(slash + 1);
    if (server.empty() || mediaId.empty()) return mxcUrl;
    std::string base = homeServerUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();
    std::ostringstream out;
    out << base << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId;
    out << "?width=" << width << "&height=" << height << "&method=" << method;
    return out.str();
}

} // namespace pcp
