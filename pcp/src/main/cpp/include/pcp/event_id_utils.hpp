#pragma once
#include <string>

namespace pcp {

bool validateEventId(const std::string& eventId);
std::string extractServerFromEventId(const std::string& eventId);
int compareEventIds(const std::string& eid1, const std::string& eid2);
std::string parseEventId(const std::string& input);
std::string buildEventId(const std::string& serverName);

} // namespace pcp
