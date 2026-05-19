#ifndef PROGRESSIVE_INVITATION_HIDE_HPP
#define PROGRESSIVE_INVITATION_HIDE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace progressive {

struct HiddenInvitation {
    std::string roomId;
    std::string roomName;
    std::string inviterName;
    std::string inviterMxid;
    int64_t hiddenAt = 0; // epoch ms when hidden
};

class InvitationHideList {
public:
    // Hide an invitation (add to blacklist).
    void hide(const HiddenInvitation& invite);

    // Unhide — remove from blacklist, restore the invitation.
    void unhide(const std::string& roomId);

    // Check if a room's invitation is hidden.
    bool isHidden(const std::string& roomId) const;

    // Get all hidden invitations.
    std::vector<HiddenInvitation> getAll() const;

    // Get count of hidden invitations.
    size_t count() const { return hidden_.size(); }

    // Export as JSON for persistence.
    std::string exportJson() const;

    // Import from JSON (restore after app restart).
    void importJson(const std::string& json);

    // Clear all.
    void clear();

private:
    std::unordered_map<std::string, HiddenInvitation> hidden_;
};

} // namespace progressive

#endif // PROGRESSIVE_INVITATION_HIDE_HPP
