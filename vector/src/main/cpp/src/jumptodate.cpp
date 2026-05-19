#include "progressive/jumptodate.hpp"
#include "progressive/json_parser.hpp"
#include <ctime>
#include <sstream>
#include <regex>
#include <cstring>

namespace progressive {

bool validateAndComputeTimestamp(JumpToDateRequest& request) {
    // YYYY-MM-DD regex
    static const std::regex dateRegex(R"(^(\d{4})-(\d{2})-(\d{2})$)");
    std::smatch match;

    if (!std::regex_match(request.dateString, match, dateRegex)) {
        request.errorMessage = "Invalid date format. Use YYYY-MM-DD.";
        return false;
    }

    int year  = std::stoi(match[1]);
    int month = std::stoi(match[2]);
    int day   = std::stoi(match[3]);

    if (month < 1 || month > 12) {
        request.errorMessage = "Invalid month. Must be 01-12.";
        return false;
    }

    if (day < 1 || day > 31) {
        request.errorMessage = "Invalid day. Must be 01-31.";
        return false;
    }

    // Days per month validation
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    auto isLeapYear = [](int y) { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); };
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) maxDay = 29;

    if (day > maxDay) {
        request.errorMessage = "Invalid day for given month.";
        return false;
    }

    request.originServerTs = dateToUnixTimestamp(year, month, day);
    return true;
}

int64_t dateToUnixTimestamp(int year, int month, int day) {
    struct tm timeinfo = {};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon  = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min  = 0;
    timeinfo.tm_sec  = 0;

    // Use timegm for UTC (POSIX). Fallback to mktime with TZ=UTC on Android.
    auto timestamp = static_cast<int64_t>(timegm(&timeinfo));

    // MSC3030 expects milliseconds
    return timestamp * 1000;
}

std::string buildMsc3030Url(const JumpToDateRequest& request) {
    std::ostringstream oss;
    oss << request.serverBaseUrl
        << "/_matrix/client/unstable/org.matrix.msc3030"
        << "/rooms/" << request.roomId
        << "/timestamp_to_event"
        << "?ts=" << request.originServerTs
        << "&dir=f";
    return oss.str();
}

JumpToDateResult parseTimestampToEventResponse(const std::string& responseBody, int httpStatus) {
    JumpToDateResult result;
    result.statusCode = httpStatus;

    if (httpStatus != 200) {
        result.success = false;

        auto errcode = parseJsonStringValue(responseBody, "errcode");
        auto error   = parseJsonStringValue(responseBody, "error");

        result.errorMessage = "Server returned " + std::to_string(httpStatus);
        if (!errcode.empty()) result.errorMessage += " (" + errcode + ")";
        if (!error.empty()) result.errorMessage += ": " + error;
        return result;
    }

    auto eventId = parseJsonStringValue(responseBody, "event_id");
    if (eventId.empty()) {
        result.success = false;
        result.errorMessage = "Response missing event_id field.";
        return result;
    }

    // Handle "event_id": "~" which means no event found after that timestamp
    if (eventId == "~") {
        result.success = false;
        result.errorMessage = "No events found after the given date.";
        return result;
    }

    result.success = true;
    result.eventId = eventId;
    return result;
}

} // namespace progressive
