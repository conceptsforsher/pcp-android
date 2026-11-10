#pragma once
#include <string>
#include <cstdint>

namespace pcp {
struct MediaProgress { std::string mxcUrl; int64_t totalBytes = 0; int64_t downloadedBytes = 0; bool isComplete = false; };
std::string buildMediaProgressJson(const MediaProgress& p);
MediaProgress parseMediaProgress(const std::string& json);
int getProgressPercent(const MediaProgress& p);
std::string formatMediaProgress(const MediaProgress& p);
}
