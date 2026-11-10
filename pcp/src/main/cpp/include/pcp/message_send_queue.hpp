#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string enqueueSend(const std::string& json);
std::string dequeueSend(const std::string& json);
std::string getQueueSize(const std::string& json);
std::string cancelSend(const std::string& json);
std::string retryFailed(const std::string& json);
