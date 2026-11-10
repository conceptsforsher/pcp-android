#pragma once
#include <string>
#include <vector>
namespace pcp {
std::string formatReadReceiptCount(int count);
std::string formatReadReceiptNames(const std::vector<std::string>& names, int maxShow=3);
int getReadReceiptAvatarsLimit(int screenWidth);
}
