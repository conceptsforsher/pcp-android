#include "progressive/proxy.hpp"
#include <sstream>
#include <regex>

namespace progressive {

ProxyConfig computeProxyConfig(
    ConnectionType connType,
    ProxyType userProxyType,
    const std::string& userProxyHost,
    int userProxyPort,
    const std::string& userProxyUsername,
    const std::string& userProxyPassword
) {
    ProxyConfig config;

    switch (connType) {
        case ConnectionType::Onion:
            config.enabled = true;
            config.type = ProxyType::Socks5;
            config.host = "127.0.0.1";
            config.port = 9050;
            break;

        case ConnectionType::I2p:
            config.enabled = true;
            config.type = ProxyType::Http;
            config.host = "127.0.0.1";
            config.port = 4444;
            break;

        case ConnectionType::Matrix:
            if (userProxyType != ProxyType::None && !userProxyHost.empty() && userProxyPort > 0) {
                config.enabled = true;
                config.type = userProxyType;
                config.host = userProxyHost;
                config.port = userProxyPort;
                config.username = userProxyUsername;
                config.password = userProxyPassword;
            }
            break;
    }

    return config;
}

bool isValidProxyHost(const std::string& host) {
    if (host.empty()) return false;
    // IPv4: x.x.x.x
    std::regex ipv4(R"(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)");
    std::smatch match;
    if (std::regex_match(host, match, ipv4)) {
        for (int i = 1; i <= 4; ++i) {
            int octet = std::stoi(match[i]);
            if (octet > 255) return false;
        }
        return true;
    }
    // Hostname: alphanumeric, dots, hyphens
    std::regex hostname(R"(^[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?(\.[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?)*$)");
    if (std::regex_match(host, hostname)) return true;
    // IPv6 in brackets
    std::regex ipv6(R"(^\[[0-9a-fA-F:]+\]$)");
    return std::regex_match(host, ipv6);
}

bool isValidProxyPort(int port) {
    return port > 0 && port <= 65535;
}

std::string ProxyConfig::toJson() const {
    std::ostringstream json;
    json << "{";
    json << R"("enabled": )" << (enabled ? "true" : "false") << ",";
    json << R"("type": ")";
    switch (type) {
        case ProxyType::Http:   json << "HTTP"; break;
        case ProxyType::Socks5: json << "SOCKS"; break;
        default:                json << "None"; break;
    }
    json << R"(",)";
    json << R"("host": ")" << host << R"(",)";
    json << R"("port": )" << port;
    if (!username.empty()) {
        json << R"(,"username": ")" << username << R"(")";
        json << R"(,"password": ")" << password << R"(")";
    }
    json << "}";
    return json.str();
}

} // namespace progressive
