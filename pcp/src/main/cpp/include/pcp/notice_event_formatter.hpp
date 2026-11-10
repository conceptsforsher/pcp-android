#pragma once
#include <string>

namespace pcp {

// Formats a room event notice into a localized string description.
// Takes JSON event data and returns a JSON result with the string key and parameters.
//
// Input: JSON with event content fields
// Output: {"type":"string","key":"notice_room_name_changed","params":["Alice","New Room"]}
//    or: {"type":"null"}
//
std::string formatNoticeEvent(const std::string& eventJson,
                               const std::string& senderName,
                               bool isDm,
                               bool isSentByCurrentUser,
                               const std::string& currentUserId);

// Format a redacted event notice
std::string formatRedactedEvent(const std::string& eventJson,
                                 const std::string& senderName);

// Format a room member change notice
std::string formatMemberNotice(const std::string& eventJson,
                                const std::string& senderName,
                                bool isDm,
                                bool isSentByCurrentUser);

// Format a displayable event (for room list preview)
std::string formatDisplayableEvent(const std::string& eventJson,
                                    const std::string& senderName,
                                    bool isDm,
                                    bool appendAuthor);

// Format thread summary
std::string formatThreadSummary(const std::string& eventJson,
                                 const std::string& latestEdition);

// Format event details (media info: dimensions, duration, file size)
// Returns JSON with string template and params
std::string formatEventDetails(const std::string& eventJson);

// Format room history visibility string
std::string formatRoomHistoryVisibility(const std::string& visibility, bool forNotice);

} // namespace pcp
