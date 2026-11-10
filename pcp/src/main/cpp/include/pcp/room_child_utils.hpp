#pragma once
#include <string>
#include <cstdint>

std::string parseChildRoomId(const std::string& json);
std::string getChildrenRooms(const std::string& json);
std::string buildChildEvent(const std::string& json);
