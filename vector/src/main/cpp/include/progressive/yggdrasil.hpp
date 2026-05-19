#ifndef PROGRESSIVE_YGGDRASIL_HPP
#define PROGRESSIVE_YGGDRASIL_HPP

#include <string>

namespace progressive {

// Check if a string is a valid Yggdrasil IPv6 address (200::/7 prefix)
bool isYggdrasilAddress(const std::string& addr);

// Check if a hostname ends with .ygg (Yggdrasil DNS domain)
bool isYggdrasilDomain(const std::string& hostname);

// Rewrite a Matrix homeserver URL to use Yggdrasil transport.
// If the homeserver has a .ygg address, resolve it to an IPv6 literal.
// Returns rewritten URL or empty string if not yggdrasil-reachable.
std::string rewriteHomeserverUrl(
    const std::string& originalUrl,  // e.g. "https://matrix.example.com"
    const std::string& yggAddress    // e.g. "200:abcd:ef01::1"
);

// Normalize a Yggdrasil address: lowercase, strip brackets if present.
std::string normalizeYggAddress(const std::string& addr);

// Build a Matrix server name from a Yggdrasil address.
// e.g. "https://[200:abcd::1]:8448"
std::string buildYggHomeserverUrl(
    const std::string& yggAddress,
    int port = 8448,
    bool tls = true
);

} // namespace progressive

#endif // PROGRESSIVE_YGGDRASIL_HPP
