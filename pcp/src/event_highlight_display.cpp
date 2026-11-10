#include "pcp/event_highlight_display.hpp"
#include <sstream>

namespace pcp {
std::string getHighlightColor(int c) { return c > 0 ? "#FF5722" : "#607D8B"; }
std::string formatHighlightBadge(int c) { if (c <= 0) return ""; return c >= 1000 ? "999+" : std::to_string(c); }
std::string getHighlightSoundHint() { return "content://settings/system/notification_sound"; }
} // namespace pcp
