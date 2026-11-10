#include "pcp/server_notice_utils.hpp"
#include <sstream>

namespace pcp {

ServerNoticeContent parseServerNotice(const std::string& json) {
    ServerNoticeContent n;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    n.adminContact = extract("admin_contact");
    n.limitType = extract("limit_type");
    n.message = extract("body");
    return n;
}

bool isServerNoticeEvent(const std::string& json) {
    return json.find("m.server_notice") != std::string::npos ||
           json.find("M_SERVER_NOTICE") != std::string::npos;
}

std::string formatServerNotice(const ServerNoticeContent& n) {
    std::ostringstream os;
    os << "Server notice: " << n.message;
    if (!n.adminContact.empty()) os << " Contact: " << n.adminContact;
    return os.str();
}

std::string buildServerNoticeTag() {
    return R"({"tags":{"m.server_notice":{}}})";
}

} // namespace pcp
