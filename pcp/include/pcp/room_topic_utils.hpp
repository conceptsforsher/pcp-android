#pragma once
#include <string>
namespace pcp {
std::string buildTopicEvent(const std::string& topic);
std::string parseTopicFromEvent(const std::string& json);
std::string truncateTopic(const std::string& topic, int maxLen=120);
std::string formatTopicPreview(const std::string& topic, int maxLen=80);
}
