#pragma once
#include <string>
#include <cstdint>

std::string getSidebarOrder(const std::string& json);
std::string pinRoom(const std::string& json);
std::string unpinRoom(const std::string& json);
std::string isPinned(const std::string& json);
std::string reorderSidebar(const std::string& json);
