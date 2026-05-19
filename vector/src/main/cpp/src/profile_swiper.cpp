#include "progressive/profile_swiper.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

void ProfileSwiper::setProfiles(const std::vector<ProfileEntry>& profiles) {
    profiles_ = profiles;
    currentIndex_ = 0;
}

SwipeState ProfileSwiper::swipeNext() {
    if (currentIndex_ < static_cast<int>(profiles_.size()) - 1) {
        ++currentIndex_;
    }
    return buildState();
}

SwipeState ProfileSwiper::swipePrev() {
    if (currentIndex_ > 0) {
        --currentIndex_;
    }
    return buildState();
}

SwipeState ProfileSwiper::jumpTo(const std::string& userId) {
    for (size_t i = 0; i < profiles_.size(); ++i) {
        if (profiles_[i].userId == userId) {
            currentIndex_ = static_cast<int>(i);
            break;
        }
    }
    return buildState();
}

SwipeState ProfileSwiper::getState() const {
    return buildState();
}

SwipeState ProfileSwiper::buildState() const {
    SwipeState state;
    state.totalProfiles = static_cast<int>(profiles_.size());
    state.currentIndex = currentIndex_;
    state.hasPrev = currentIndex_ > 0;
    state.hasNext = currentIndex_ < state.totalProfiles - 1;

    if (currentIndex_ >= 0 && currentIndex_ < state.totalProfiles) {
        state.currentUserId = profiles_[currentIndex_].userId;
        if (state.hasPrev) state.prevUserId = profiles_[currentIndex_ - 1].userId;
        if (state.hasNext) state.nextUserId = profiles_[currentIndex_ + 1].userId;
    }

    return state;
}

void ProfileSwiper::sortByName(std::vector<ProfileEntry>& profiles) {
    std::sort(profiles.begin(), profiles.end(), [](const ProfileEntry& a, const ProfileEntry& b) {
        return a.displayName < b.displayName;
    });
}

void ProfileSwiper::sortByMessageCount(std::vector<ProfileEntry>& profiles) {
    std::sort(profiles.begin(), profiles.end(), [](const ProfileEntry& a, const ProfileEntry& b) {
        return a.messageCount > b.messageCount;
    });
}

void ProfileSwiper::sortByServer(std::vector<ProfileEntry>& profiles) {
    std::sort(profiles.begin(), profiles.end(), [](const ProfileEntry& a, const ProfileEntry& b) {
        if (a.serverName != b.serverName) return a.serverName < b.serverName;
        return a.displayName < b.displayName;
    });
}

std::string ProfileSwiper::stateToJson(const SwipeState& state) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("currentIndex": )" << state.currentIndex << ",";
    json << R"("totalProfiles": )" << state.totalProfiles << ",";
    json << R"("hasNext": )" << (state.hasNext ? "true" : "false") << ",";
    json << R"("hasPrev": )" << (state.hasPrev ? "true" : "false") << ",";
    json << R"("currentUserId": ")" << esc(state.currentUserId) << R"(")";
    if (state.hasNext) json << R"(,"nextUserId": ")" << esc(state.nextUserId) << R"(")";
    if (state.hasPrev) json << R"(,"prevUserId": ")" << esc(state.prevUserId) << R"(")";
    json << "}";
    return json.str();
}

} // namespace progressive
