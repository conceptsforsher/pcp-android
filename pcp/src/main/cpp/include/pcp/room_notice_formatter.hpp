#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string formatServerNotice(const std::string& json);
std::string formatRoomNotice(const std::string& json);
std::string isAdminNotice(const std::string& json);
std::string parseNoticeParams(const std::string& json);
std::string buildNoticeHtml(const std::string& json);
