#pragma once

#include <string>

namespace pcp {

// Build a weather API request URL for a given city/location.
// Uses wttr.in (free, no API key) or OpenWeatherMap if token provided.
// Format: https://wttr.in/Moscow?format=j1
std::string buildWeatherUrl(const std::string& location, const std::string& apiToken = "");

// Parse wttr.in JSON response into a human-readable weather summary.
// Input: JSON from wttr.in/?format=j1
// Output: multi-line weather summary string
std::string parseWeatherResponse(const std::string& json);

// Parse OpenWeatherMap JSON response.
// Input: JSON from api.openweathermap.org
// Output: weather summary string
std::string parseOpenWeatherResponse(const std::string& json);

// Format weather info for the alarm screen display.
std::string formatWeatherForAlarm(const std::string& location, const std::string& weatherSummary);

// Encode a string for URL (basic implementation).


} // namespace pcp
