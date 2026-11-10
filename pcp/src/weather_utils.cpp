#include "pcp/weather_utils.hpp"
#include "pcp/url_tools.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {


std::string buildWeatherUrl(const std::string& location, const std::string& apiToken) {
    if (!apiToken.empty()) {
        // OpenWeatherMap
        std::ostringstream os;
        os << "https://api.openweathermap.org/data/2.5/weather?q="
           << urlEncode(location) << "&appid=" << apiToken << "&units=metric&lang=ru";
        return os.str();
    }
    // wttr.in (free, no API key)
    std::ostringstream os;
    os << "https://wttr.in/" << urlEncode(location) << "?format=j1&lang=ru";
    return os.str();
}

static std::string extractJsonValue(const std::string& json, const std::string& key) {
    auto search = "\"" + key + "\":\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) {
        search = "\"" + key + "\": \"";
        pos = json.find(search);
    }
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

static double extractJsonDouble(const std::string& json, const std::string& key) {
    auto search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) {
        search = "\"" + key + "\": ";
        pos = json.find(search);
    }
    if (pos == std::string::npos) return 0.0;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    try {
        return std::stod(json.substr(pos));
    } catch (...) { return 0.0; }
}

std::string parseWeatherResponse(const std::string& json) {
    // Parse wttr.in j1 format
    // current_condition[0]: temp_C, weatherDesc[0].value, humidity, windspeedKmph, FeelsLikeC
    std::ostringstream os;

    auto tempC = extractJsonValue(json, "temp_C");
    auto weatherDesc = extractJsonValue(json, "value");
    auto humidity = extractJsonValue(json, "humidity");
    auto windSpeed = extractJsonValue(json, "windspeedKmph");
    auto feelsLike = extractJsonValue(json, "FeelsLikeC");

    if (!tempC.empty()) {
        os << "Temperature: " << tempC << "°C\n";
    }
    if (!feelsLike.empty()) {
        os << "Feels like: " << feelsLike << "°C\n";
    }
    if (!weatherDesc.empty()) {
        os << "Conditions: " << weatherDesc << "\n";
    }
    if (!humidity.empty()) {
        os << "Humidity: " << humidity << "%\n";
    }
    if (!windSpeed.empty()) {
        os << "Wind: " << windSpeed << " km/h\n";
    }

    // Try to get forecast for next few hours
    auto forecastPos = json.find("\"weather\"");
    if (forecastPos != std::string::npos) {
        auto nextHourly = json.find("\"hourly\"", forecastPos);
        if (nextHourly != std::string::npos) {
            // Find the first hourly entry after forecast
            auto hourlyTemp = extractJsonValue(json.substr(nextHourly), "tempC");
            auto hourlyDesc = extractJsonValue(json.substr(nextHourly), "weatherDesc");
            auto hourlyDescValue = json.find("\"value\"", nextHourly);
            if (hourlyDescValue != std::string::npos) {
                auto v1 = json.find('"', hourlyDescValue + 8);
                auto v2 = json.find('"', v1 + 1);
                if (v1 != std::string::npos && v2 != std::string::npos) {
                    hourlyDesc = json.substr(v1 + 1, v2 - v1 - 1);
                }
            }
            if (!hourlyTemp.empty()) {
                os << "\nNext hour: " << hourlyTemp << "°C";
                if (!hourlyDesc.empty()) os << ", " << hourlyDesc;
                os << "\n";
            }
        }
    }

    if (os.str().empty()) return "Weather data unavailable";
    return os.str();
}

std::string parseOpenWeatherResponse(const std::string& json) {
    std::ostringstream os;

    // Extract from OpenWeatherMap format
    auto temp = extractJsonDouble(json, "temp");
    auto feelsLike = extractJsonDouble(json, "feels_like");
    auto humidity = extractJsonDouble(json, "humidity");
    auto pressure = extractJsonDouble(json, "pressure");

    if (temp > 0 || temp < 0) {
        int tempInt = static_cast<int>(temp);
        os << "Temperature: " << tempInt << "°C\n";
    }
    if (feelsLike > 0 || feelsLike < 0) {
        int fl = static_cast<int>(feelsLike);
        os << "Feels like: " << fl << "°C\n";
    }
    if (humidity > 0) {
        os << "Humidity: " << static_cast<int>(humidity) << "%\n";
    }
    if (pressure > 0) {
        os << "Pressure: " << static_cast<int>(pressure) << " hPa\n";
    }

    // Weather description from the weather array
    auto wdPos = json.find("\"description\"");
    if (wdPos != std::string::npos) {
        auto v1 = json.find('"', wdPos + 14);
        auto v2 = json.find('"', v1 + 1);
        if (v1 != std::string::npos && v2 != std::string::npos) {
            std::string desc = json.substr(v1 + 1, v2 - v1 - 1);
            os << "Conditions: " << desc << "\n";
        }
    }

    // Wind
    auto windSpeed = extractJsonDouble(json, "speed");
    if (windSpeed > 0) {
        os << "Wind: " << static_cast<int>(windSpeed) << " m/s\n";
    }

    if (os.str().empty()) return "Weather data unavailable";
    return os.str();
}

std::string formatWeatherForAlarm(const std::string& location, const std::string& weatherSummary) {
    std::ostringstream os;
    os << "=== Weather for " << location << " ===\n\n";
    os << weatherSummary;
    return os.str();
}

} // namespace pcp
