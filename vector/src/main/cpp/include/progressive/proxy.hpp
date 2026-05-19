#ifndef PROGRESSIVE_PROXY_HPP
#define PROGRESSIVE_PROXY_HPP

#include <string>

namespace progressive {

enum class ConnectionType { Matrix, Onion, I2p };
enum class ProxyType { None, Http, Socks5 };

struct ProxyConfig {
    bool enabled = false;
    ProxyType type = ProxyType::None;
    std::string host;
    int port = 0;
    std::string username;
    std::string password;

    // Format as JSON for Kotlin consumption
    std::string toJson() const;
};

// Determine proxy configuration based on connection type and settings.
// For Onion: SOCKS5 127.0.0.1:9050
// For I2P: HTTP 127.0.0.1:4444
// For Matrix: user-defined proxy from settings
ProxyConfig computeProxyConfig(
    ConnectionType connType,
    ProxyType userProxyType,
    const std::string& userProxyHost,
    int userProxyPort,
    const std::string& userProxyUsername,
    const std::string& userProxyPassword
);

// Validate a proxy host string (IP or hostname)
bool isValidProxyHost(const std::string& host);

// Validate proxy port (1-65535)
bool isValidProxyPort(int port);

} // namespace progressive

#endif // PROGRESSIVE_PROXY_HPP
