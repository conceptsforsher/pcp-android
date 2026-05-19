#ifndef PROGRESSIVE_PROFILE_SWIPER_HPP
#define PROGRESSIVE_PROFILE_SWIPER_HPP

#include <string>
#include <vector>

namespace progressive {

struct ProfileEntry {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string serverName;
    int messageCount = 0;
    int index = 0;          // position in sorted list
};

struct SwipeState {
    int currentIndex = 0;
    int totalProfiles = 0;
    bool hasNext = false;
    bool hasPrev = false;
    std::string nextUserId;
    std::string prevUserId;
    std::string currentUserId;
};

class ProfileSwiper {
public:
    // Set the profile list with current sort order.
    void setProfiles(const std::vector<ProfileEntry>& profiles);

    // Navigate to next profile.
    SwipeState swipeNext();

    // Navigate to previous profile.
    SwipeState swipePrev();

    // Jump to a specific user.
    SwipeState jumpTo(const std::string& userId);

    // Get current state.
    SwipeState getState() const;

    // Sort profiles by name (A-Z).
    static void sortByName(std::vector<ProfileEntry>& profiles);

    // Sort profiles by message count.
    static void sortByMessageCount(std::vector<ProfileEntry>& profiles);

    // Sort profiles by join date (server name group).
    static void sortByServer(std::vector<ProfileEntry>& profiles);

    // Format swipe state as JSON.
    static std::string stateToJson(const SwipeState& state);

private:
    std::vector<ProfileEntry> profiles_;
    int currentIndex_ = 0;

    SwipeState buildState() const;
};

} // namespace progressive

#endif // PROGRESSIVE_PROFILE_SWIPER_HPP
