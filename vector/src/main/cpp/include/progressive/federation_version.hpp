#pragma once

#include <string>

namespace progressive {

// Matrix Federation Version API response.
//
// Ref: https://matrix.org/docs/spec/server_server/latest#get-matrix-federation-v1-version
//
// Original Kotlin (FederationVersion.kt:24-33):
//   data class FederationVersion(
//       val name: String?,    // Arbitrary name that identifies this implementation
//       val version: String?  // Version of this implementation
//   )

struct FederationVersion {
    std::string name;     // Arbitrary name that identifies this implementation
    std::string version;  // Version of this implementation (format varies)
};

// Original Kotlin (FederationGetVersionResult.kt:24-27):
//   @JsonClass(generateAdapter = true)
//   internal data class FederationGetVersionResult(
//       @Json(name = "server") val server: FederationGetVersionServer?
//   )
//
// JSON: {"server": {"name": "Synapse", "version": "1.62.0"}}

// Parse federation version JSON from GET /_matrix/federation/v1/version
FederationVersion parseFederationVersion(const std::string& json);

// Convert federation version to JSON
std::string federationVersionToJson(const FederationVersion& version);

} // namespace progressive
