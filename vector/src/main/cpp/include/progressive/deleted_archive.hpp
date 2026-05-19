#ifndef PROGRESSIVE_DELETED_ARCHIVE_HPP
#define PROGRESSIVE_DELETED_ARCHIVE_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct DeletedEvent {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string senderName;
    std::string body;           // pre-deletion content
    std::string formattedBody;  // HTML if available
    std::string msgType;
    std::string timestamp;      // ISO 8601 when it was sent
    int64_t originServerTs = 0;
    int64_t deletedAt = 0;      // when it was deleted (epoch ms)
    std::string deletedBy;      // who deleted it
};

class DeletedMessageArchive {
public:
    // Archive a message before it's removed from timeline.
    void archive(const DeletedEvent& event);

    // Get all deleted events for a room.
    std::vector<DeletedEvent> getByRoom(const std::string& roomId) const;

    // Get all deleted events.
    std::vector<DeletedEvent> getAll() const;

    // Permanently remove from archive.
    void purge(const std::string& eventId);

    // Clear entire archive.
    void clear();

    // Export as JSON for inclusion in chat export.
    std::string exportJson() const;

    size_t count() const { return entries_.size(); }

private:
    std::vector<DeletedEvent> entries_;
};

} // namespace progressive

#endif // PROGRESSIVE_DELETED_ARCHIVE_HPP
