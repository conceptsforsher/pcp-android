#pragma once
#include <string>

namespace pcp {

// Get color for highlight level
std::string getHighlightColor(int highlightCount);

// Format highlight badge text
std::string formatHighlightBadge(int count);

// Get highligh notification sound hint
std::string getHighlightSoundHint();

} // namespace pcp
