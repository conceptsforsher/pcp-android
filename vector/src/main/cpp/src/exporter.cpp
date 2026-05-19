#include "progressive/exporter.hpp"
#include <sstream>
#include <ctime>
#include <cstring>

namespace progressive {

std::string escapeJson(const std::string& input) {
    std::string out;
    out.reserve(input.size());
    for (char c : input) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out += c;
        }
    }
    return out;
}

std::string formatEventHtml(const ExportEvent& event, bool isContinuation) {
    std::ostringstream html;

    if (!isContinuation) {
        html << "<div class=\"mx_EventTile\">\n";
        html << "  <div class=\"mx_EventTile_info\">\n";
        html << "    <span class=\"mx_EventTile_sender\">" << escapeHtml(event.senderName) << "</span>\n";
        if (!event.timestamp.empty()) {
            html << "    <span class=\"mx_MessageTimestamp\">" << escapeHtml(event.timestamp) << "</span>\n";
        }
        html << "  </div>\n";
    } else {
        html << "<div class=\"mx_EventTile mx_EventTile_continuation\">\n";
    }

    html << "  <div class=\"mx_EventTile_body\">\n";

    if (event.msgType == "m.image" || event.msgType == "m.video" || event.msgType == "m.file" || event.msgType == "m.audio") {
        html << "    <div class=\"mx_EventTile_attachment\">\n";
        html << "      <span class=\"mx_Attachment_name\">" << escapeHtml(event.fileName.empty() ? event.msgType.substr(2) : event.fileName) << "</span>\n";
        if (event.mediaSize > 0) {
            html << "      <span class=\"mx_Attachment_size\">" << std::to_string(event.mediaSize) << " bytes</span>\n";
        }
        html << "    </div>\n";
    }

    if (!event.body.empty()) {
        html << "    <div class=\"mx_EventTile_content\">\n";
        html << "      " << escapeHtml(event.body) << "\n";
        html << "    </div>\n";
    }

    if (event.relationType == "m.annotation" && !event.sourceEventId.empty()) {
        html << "    <div class=\"mx_EventTile_reaction\">\n";
        html << "      " << escapeHtml(event.body) << " (reaction to " << escapeHtml(event.sourceEventId) << ")\n";
        html << "    </div>\n";
    }

    html << "  </div>\n";
    html << "</div>\n";

    return html.str();
}

std::string formatEventPlainText(const ExportEvent& event) {
    std::ostringstream text;

    if (!event.timestamp.empty()) {
        text << event.timestamp << " - ";
    }
    text << event.senderName << ": ";

    if (event.msgType == "m.image" || event.msgType == "m.video" || event.msgType == "m.file" || event.msgType == "m.audio") {
        text << "[" << event.msgType.substr(2) << " attached";
        if (!event.fileName.empty()) text << ": " << event.fileName;
        text << "]";
    }

    if (!event.body.empty()) {
        text << " " << event.body;
    }

    if (event.relationType == "m.reference" && !event.sourceEventId.empty()) {
        text << " (in reply to " << event.sourceEventId << ")";
    }

    text << "\n";
    return text.str();
}

std::string formatEventJson(const ExportEvent& event) {
    std::ostringstream json;

    json << "  {\n";
    json << "    \"eventId\": \"" << escapeJson(event.eventId) << "\",\n";
    json << "    \"senderId\": \"" << escapeJson(event.senderId) << "\",\n";
    json << "    \"senderName\": \"" << escapeJson(event.senderName) << "\",\n";
    json << "    \"timestamp\": \"" << escapeJson(event.timestamp) << "\",\n";
    if (!event.eventType.empty())
        json << "    \"eventType\": \"" << escapeJson(event.eventType) << "\",\n";
    if (!event.msgType.empty())
        json << "    \"msgType\": \"" << escapeJson(event.msgType) << "\",\n";
    json << "    \"body\": \"" << escapeJson(event.body) << "\",\n";
    if (!event.formattedBody.empty())
        json << "    \"formattedBody\": \"" << escapeJson(event.formattedBody) << "\",\n";
    if (!event.relationType.empty())
        json << "    \"relationType\": \"" << escapeJson(event.relationType) << "\",\n";
    if (!event.sourceEventId.empty())
        json << "    \"sourceEventId\": \"" << escapeJson(event.sourceEventId) << "\",\n";
    if (!event.mediaUrl.empty())
        json << "    \"mediaUrl\": \"" << escapeJson(event.mediaUrl) << "\",\n";
    json << "    \"isEncrypted\": " << (event.isEncrypted ? "true" : "false");
    if (event.mediaSize > 0) {
        json << ",\n    \"mediaSize\": " << event.mediaSize;
    }
    json << "\n  }";

    return json.str();
}

std::string buildHtmlDocument(
    const std::string& roomName,
    const std::string& roomTopic,
    const std::string& exportDate,
    const std::vector<ExportEvent>& events
) {
    std::ostringstream html;

    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "<title>" << escapeHtml(roomName) << " — Chat Export</title>\n";
    html << R"(<style>
body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; margin: 0; padding: 16px; background: #f5f5f5; }
.mx_ExportHeader { background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
.mx_ExportHeader h1 { margin: 0 0 8px; font-size: 1.5em; }
.mx_ExportHeader p { margin: 4px 0; color: #666; font-size: 0.9em; }
.mx_EventTile { background: #fff; border-radius: 8px; padding: 12px 16px; margin-bottom: 8px; box-shadow: 0 1px 2px rgba(0,0,0,0.05); }
.mx_EventTile_continuation { margin-top: -4px; border-radius: 0 0 8px 8px; }
.mx_EventTile_info { margin-bottom: 6px; }
.mx_EventTile_sender { font-weight: 600; color: #333; margin-right: 8px; }
.mx_MessageTimestamp { color: #999; font-size: 0.85em; }
.mx_EventTile_body { color: #222; line-height: 1.5; }
.mx_EventTile_attachment { background: #f0f0f0; border-radius: 4px; padding: 8px; margin-bottom: 8px; }
.mx_Attachment_name { font-weight: 500; }
.mx_Attachment_size { color: #999; margin-left: 8px; }
.mx_EventTile_reaction { font-style: italic; color: #666; }
.mx_EventTile_content { white-space: pre-wrap; word-wrap: break-word; }
hr { border: none; border-top: 1px solid #e0e0e0; margin: 16px 0; }
</style>)" << "\n";
    html << "</head>\n<body>\n";

    html << "<div class=\"mx_ExportHeader\">\n";
    html << "  <h1>" << escapeHtml(roomName) << "</h1>\n";
    if (!roomTopic.empty()) {
        html << "  <p>" << escapeHtml(roomTopic) << "</p>\n";
    }
    html << "  <p>Exported: " << exportDate << "</p>\n";
    html << "  <p>Total messages: " << events.size() << "</p>\n";
    html << "</div>\n";

    if (events.empty()) {
        html << "<p>No messages to export.</p>\n";
    }

    std::string prevSender;
    for (const auto& event : events) {
        bool isContinuation = (event.senderId == prevSender);
        html << formatEventHtml(event, isContinuation);
        prevSender = event.senderId;
    }

    html << "<hr>\n<p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>\n";
    html << "</body>\n</html>";
    return html.str();
}

std::string buildPlainTextDocument(
    const std::string& roomName,
    const std::string& exportDate,
    const std::vector<ExportEvent>& events
) {
    std::ostringstream text;

    text << roomName << "\n";
    for (size_t i = 0; i < roomName.size(); ++i) text << "=";
    text << "\n";
    text << "Exported: " << exportDate << "\n";
    text << "Messages: " << events.size() << "\n\n";

    for (const auto& event : events) {
        text << formatEventPlainText(event);
    }

    return text.str();
}

std::string buildJsonDocument(
    const std::string& roomName,
    const std::string& roomTopic,
    const std::string& exportDate,
    const std::string& roomCreator,
    const std::vector<ExportEvent>& events
) {
    std::ostringstream json;

    json << "{\n";
    json << "  \"roomName\": \"" << escapeJson(roomName) << "\",\n";
    if (!roomTopic.empty())
        json << "  \"roomTopic\": \"" << escapeJson(roomTopic) << "\",\n";
    if (!roomCreator.empty())
        json << "  \"roomCreator\": \"" << escapeJson(roomCreator) << "\",\n";
    json << "  \"exportDate\": \"" << escapeJson(exportDate) << "\",\n";
    json << "  \"messageCount\": " << events.size() << ",\n";
    json << "  \"messages\": [\n";

    for (size_t i = 0; i < events.size(); ++i) {
        json << formatEventJson(events[i]);
        if (i + 1 < events.size()) json << ",";
        json << "\n";
    }

    json << "  ]\n";
    json << "}\n";

    return json.str();
}

} // namespace progressive
