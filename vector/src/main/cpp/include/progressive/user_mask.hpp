#ifndef PROGRESSIVE_USER_MASK_HPP
#define PROGRESSIVE_USER_MASK_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct UserMask {
    std::string originalMxid;      // @user:server
    std::string displayName;       // override display name
    std::string avatarUrl;         // override avatar (local file or mxc://)
    std::string overrideMxid;      // override shown MXID (empty = don't override)
};

class UserMaskRegistry {
public:
    // Add or update a mask for a user.
    void setMask(const UserMask& mask);

    // Remove mask for a user.
    void removeMask(const std::string& mxid);

    // Get mask for a user. Returns nullptr if not masked.
    const UserMask* getMask(const std::string& mxid) const;

    // Check if a user is masked.
    bool isMasked(const std::string& mxid) const;

    // Get all masks as JSON array.
    std::string exportJson() const;

    // Load masks from JSON array string.
    void importJson(const std::string& json);

    // Clear all masks.
    void clear();

    size_t count() const { return masks_.size(); }

private:
    std::unordered_map<std::string, UserMask> masks_;
    static std::string normalizeMxid(const std::string& mxid);
};

// Validate a Matrix user ID format (@user:server)
bool isValidMxid(const std::string& mxid);

// Extract display name from a mask (returns original if no mask)
std::string resolveDisplayName(const std::string& mxid, const std::string& originalName,
                               const UserMaskRegistry& registry);

// Extract avatar URL from a mask (returns original if no mask)
std::string resolveAvatarUrl(const std::string& mxid, const std::string& originalUrl,
                             const UserMaskRegistry& registry);

} // namespace progressive

#endif // PROGRESSIVE_USER_MASK_HPP
