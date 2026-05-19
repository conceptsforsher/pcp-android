#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "progressive/crypto_models.hpp"

namespace progressive {

// ================================================================
// Device Manager — full device management (Element Android port)
//
// Based on original Element Android sources:
//   DeviceInfo.kt — basic device info (user_id, device_id, display_name,
//     last_seen_ts, last_seen_ip, last_seen_user_agent)
//   CryptoDeviceInfo.kt — crypto device (algorithms, keys, signatures,
//     trustLevel, fingerprint/identityKey extraction)
//   DeviceTrustLevel.kt — crossSigningVerified + locallyVerified
//   CryptoService.kt — fetchDevicesList, getMyDevicesInfo,
//     setDeviceName, deleteDevice, getUserDevices, getCryptoDeviceInfo
//   DevicesListResponse.kt — /devices API response
//
// Covers:
//   1. Device list fetching/parsing (/devices API)
//   2. Device info management (name, last seen, user agent)
//   3. Crypto device info (fingerprint, identity key, trust levels)
//   4. Device rename (PUT /devices/{deviceId})
//   5. Device deletion (DELETE /devices/{deviceId} + UIA)
//   6. Batch device deletion
//   7. Device verification trust levels
//   8. Device inactivity detection
//   9. Device sorting/filtering
//  10. Device fingerprint formatting
// ================================================================

// ---- Device Trust Level ----
// Original: DeviceTrustLevel.kt (crossSigningVerified, locallyVerified)

struct DeviceTrustLevel {
    bool crossSigningVerified = false;
    bool locallyVerified = false;       // null in original = false here

    bool isVerified() const { return crossSigningVerified || locallyVerified; }
    bool isCrossSigningVerified() const { return crossSigningVerified; }
    bool isLocallyVerified() const { return locallyVerified; }
    bool isUnknown() const { return !crossSigningVerified && !locallyVerified; }
};

// ---- Device Info ----
// Original: DeviceInfo.kt (userId, deviceId, displayName, lastSeenTs,
//   lastSeenIp, lastSeenUserAgent, unstableLastSeenUserAgent)


// ---- Crypto Device Info ----
// Original: CryptoDeviceInfo.kt (deviceId, userId, algorithms, keys,
//   signatures, unsigned, trustLevel, isBlocked, firstTimeSeenLocalTs)


// ---- Devices List Response ----
// Original: DevicesListResponse (devices: List<DeviceInfo>)


// ---- Device Deletion Request ----

struct DeviceDeletionRequest {
    std::string deviceId;
    std::string authType;            // "m.login.password" for UIA
    std::string authSession;         // UIA session ID
    std::string password;            // User password for UIA
};

// ---- Device Rename Request ----

struct DeviceRenameRequest {
    std::string deviceId;
    std::string newDisplayName;
};

// ---- Device Sort Mode ----

enum class DeviceSortMode {
    BY_NAME = 0,              // Alphabetical by display name
    BY_LAST_SEEN = 1,         // Most recently seen first
    BY_TRUST_LEVEL = 2,       // Verified first, then by name
};

// ---- Device Filter ----

struct DeviceFilter {
    bool verifiedOnly = false;
    bool unverifiedOnly = false;
    bool blockedOnly = false;
    bool inactiveOnly = false;       // Not seen in 7 days (default)
    int inactivityDays = 7;
    bool currentDeviceFirst = true;  // Show current device at top
    std::string currentDeviceId;
};

// ---- Device Manager ----

class DeviceManager {
public:
    DeviceManager();

    // ====== Device List ======

    // Parse /devices API response.
    // Original: CryptoService.fetchDevicesList() → DevicesListResponse
    DevicesListResponse parseDevicesList(const std::string& json);

    // Parse a single device info from JSON.
    // Original: CryptoService.fetchDeviceInfo(deviceId) → DeviceInfo
    DeviceInfo parseDeviceInfo(const std::string& deviceId, const std::string& json);

    // Parse crypto device info from JSON.
    // Original: getCryptoDeviceInfo(userId, deviceId) → CryptoDeviceInfo
    CryptoDeviceInfo parseCryptoDeviceInfo(const std::string& deviceId, const std::string& userId, const std::string& json);

    // ====== Device Rename ======

    // Build device rename request body.
    // Original: CryptoService.setDeviceName(deviceId, deviceName)
    // PUT /devices/{deviceId} → {"display_name":"New Name"}
    std::string buildRenameRequest(const DeviceRenameRequest& req) const;

    // ====== Device Deletion ======

    // Build single device deletion request with UIA.
    // Original: CryptoService.deleteDevice(deviceId, uiaInterceptor)
    std::string buildDeleteRequest(const DeviceDeletionRequest& req) const;

    // Build batch device deletion request.
    // Original: CryptoService.deleteDevices(deviceIds, uiaInterceptor)
    std::string buildBatchDeleteRequest(const std::vector<DeviceDeletionRequest>& requests) const;

    // Check if deletion requires User-Interactive Authentication.
    bool requiresUia(const std::string& deleteResponseJson) const;

    // ====== Device Verification ======

    // Format device trust level for display.
    // Original: DeviceTrustLevel.isVerified() / isCrossSigningVerified()
    std::string formatTrustLevel(const DeviceTrustLevel& level) const;

    // Get trust label for UI ("Verified", "Cross-signing verified", "Not verified").
    std::string getTrustLabel(const DeviceTrustLevel& level) const;

    // ====== Device Fingerprint ======

    // Format device fingerprint (Ed25519 key) for display.
    // Groups of 4 uppercase chars separated by spaces.
    // Original: getFingerprintHumanReadable() — 4-char groups
    std::string formatFingerprint(const std::string& rawKey) const;

    // Get short device key (first 8 chars for UI).
    std::string formatShortKey(const std::string& rawKey) const;

    // ====== Device Inactivity ======

    // Check if a device is inactive (not seen for N days).
    // Original: used in device list to show "Inactive" badge
    bool isDeviceInactive(int64_t lastSeenTs, int inactivityDays = 7) const;

    // Format last seen time as relative string.
    std::string formatLastSeen(int64_t lastSeenTs) const;

    // Check if device version meets minimum requirement.
    bool satisfiesMinVersion(const std::string& clientVersion, const std::string& minRequired) const;

    // ====== Sorting & Filtering ======

    // Sort device list.
    void sortDevices(std::vector<DeviceInfo>& devices, DeviceSortMode mode) const;
    void sortCryptoDevices(std::vector<CryptoDeviceInfo>& devices, DeviceSortMode mode) const;

    // Filter device list.
    std::vector<DeviceInfo> filterDevices(const std::vector<DeviceInfo>& devices,
                                           const DeviceFilter& filter) const;

    // ====== Serialization ======

    // Export device info as JSON.
    std::string deviceToJson(const DeviceInfo& device) const;

    // Export crypto device info as JSON.
    std::string cryptoDeviceToJson(const CryptoDeviceInfo& device) const;

    // Export device list as JSON array.
    std::string devicesToJson(const std::vector<DeviceInfo>& devices) const;
    std::string cryptoDevicesToJson(const std::vector<CryptoDeviceInfo>& devices) const;

    // Export device trust level as JSON.
    std::string trustLevelToJson(const DeviceTrustLevel& level) const;

private:
    // JSON extraction helpers.
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

} // namespace progressive
