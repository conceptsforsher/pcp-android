#pragma once
#include <string>
#include <cstdint>

std::string getMediaEvents(const std::string& json);
std::string groupByDate(const std::string& json);
std::string getThumbForEvent(const std::string& json);
std::string getMediaCount(const std::string& json);
std::string formatGalleryView(const std::string& json);
