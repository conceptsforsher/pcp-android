#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string gossipKeyRequest(const std::string& json);
std::string receiveGossip(const std::string& json);
std::string getPendingGossips(const std::string& json);
std::string cancelGossip(const std::string& json);
std::string gossipCount(const std::string& json);
