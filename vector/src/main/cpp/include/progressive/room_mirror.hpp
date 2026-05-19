#ifndef PROGRESSIVE_ROOM_MIRROR_HPP
#define PROGRESSIVE_ROOM_MIRROR_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct MirrorConfig {
    std::string sourceRoomId;
    std::string sourceRoomName;
    std::string mirrorRoomId;      // target room (existing or to-be-created)
    std::string mirrorRoomName;
    bool enabled = false;
    bool useDolls = false;         // auto-register doll accounts
    std::string homeserverUrl;     // for doll registration
    std::string adminToken;        // admin access token for doll reg
};

struct MirrorMessage {
    std::string senderName;
    std::string senderMxid;
    std::string sourceRoomName;
    std::string body;              // original message body
    std::string msgType;
    int64_t timestamp = 0;
};

struct DollAccount {
    std::string originalMxid;      // @alice:matrix.org
    std::string dollMxid;          // @alicedoll:server.com
    std::string dollToken;         // access token for doll account
    std::string homeserverUrl;
};

class RoomMirrorManager {
public:
    // Add a mirror configuration.
    void addMirror(const MirrorConfig& config);

    // Remove a mirror by source room ID.
    void removeMirror(const std::string& sourceRoomId);

    // Get all mirrors for a source room.
    std::vector<const MirrorConfig*> getMirrorsForSource(const std::string& sourceRoomId) const;

    // Get all mirrors that target a specific room.
    std::vector<const MirrorConfig*> getMirrorsForTarget(const std::string& mirrorRoomId) const;

    // Enable/disable a mirror.
    void setEnabled(const std::string& sourceRoomId, bool enabled);

    // Check if a room has mirroring enabled.
    bool isMirroring(const std::string& sourceRoomId) const;

    // Format a mirror message: "User XY wrote in room YX: body"
    static std::string formatMirrorMessage(const MirrorMessage& msg);

    // Generate a doll MXID from original: @alice:matrix.org → @alicedoll:server.com
    static std::string generateDollMxid(const std::string& originalMxid, const std::string& targetServer);

    // Check if a doll MXID is unique (not conflicting with existing users).
    // Validation: must match @username:server format, username not reserved.
    static bool isValidDollMxid(const std::string& mxid);

    // Generate a username from MXID: @alice:matrix.org → "alice"
    static std::string extractUsername(const std::string& mxid);

    // Register a doll account (returns the DollAccount info if successful).
    // Actual registration is done via Kotlin/Matrix SDK — C++ provides the config.
    static DollAccount prepareDollRegistration(
        const std::string& originalMxid,
        const std::string& targetHomeserver,
        const std::string& adminToken
    );

    // Export all mirrors as JSON.
    std::string exportJson() const;

    // Import mirrors from JSON.
    void importJson(const std::string& json);

    // Clear all mirrors.
    void clear();

    size_t mirrorCount() const { return mirrors_.size(); }

private:
    // key: sourceRoomId → MirrorConfig
    std::unordered_map<std::string, MirrorConfig> mirrors_;
};

// Check if a homeserver URL likely supports open registration (no email/captcha).
// This is a heuristic — C++ can't actually check, but provides the config flag.
// The actual check happens via Kotlin's /.well-known/matrix/client or /register endpoint.
struct HomeserverCapability {
    bool registrationEnabled = false;
    bool emailRequired = true;
    bool captchaRequired = true;
};

// Parse a /.well-known response to determine registration capabilities.
HomeserverCapability parseRegistrationCapabilities(const std::string& wellKnownJson);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_MIRROR_HPP
