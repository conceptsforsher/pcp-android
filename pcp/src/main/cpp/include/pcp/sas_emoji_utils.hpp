#pragma once
#include <string>
#include <vector>
namespace pcp {
struct SasEmojiData { std::string emoji; std::string name; };
std::vector<SasEmojiData> getSasEmojiList();
SasEmojiData getSasEmojiByIndex(int idx);
std::string formatSasEmojiDisplay(const std::vector<int>& indices);
}
