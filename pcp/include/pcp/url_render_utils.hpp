#pragma once
#include <string>
namespace pcp {
std::string buildUrlPreviewHtml(const std::string& url, const std::string& title, const std::string& desc, const std::string& image, const std::string& site);
std::string formatUrlForDisplay(const std::string& url, int maxLen=50);
}
