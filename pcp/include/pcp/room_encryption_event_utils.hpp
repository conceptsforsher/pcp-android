#pragma once
#include <string>

namespace pcp {

struct RoomEncryptionConfig {
    std::string algorithm;          // "m.megolm.v1.aes-sha2"
    int rotationPeriodMs = 604800000;  // 7 days
    int rotationPeriodMsgs = 100;
};

// Parse m.room.encryption state event
RoomEncryptionConfig parseRoomEncryption(const std::string& json);

// Build m.room.encryption event
std::string buildRoomEncryptionEvent(const RoomEncryptionConfig& cfg);

// Format encryption config for display
std::string formatEncryptionConfig(const RoomEncryptionConfig& cfg);

} // namespace pcp
