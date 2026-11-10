#pragma once
#include <string>
#include <cstdint>

std::string algorithm;      // "m.megolm.v1.aes-sha2"(const std::string& json);
std::string senderKey;      // curve25519 key(const std::string& json);
std::string sessionId;      // megolm session ID(const std::string& json);
std::string ciphertext;     // encrypted payload(const std::string& json);
std::string deviceId;       // sender's device(const std::string& json);
std::string EncryptionInfo parseEncryptedEvent(const std::string& json);
std::string bool isEncryptedEvent(const std::string& json);
std::string buildEncryptedContent(const std(const std::string& json);
std::string string& ciphertext, const std(const std::string& json);
std::string string& senderKey,(const std::string& json);
std::string const std(const std::string& json);
std::string string& sessionId, const std(const std::string& json);
std::string string& deviceId);(const std::string& json);
std::string formatEncryptionInfo(const EncryptionInfo& info);(const std::string& json);
std::string bool isMegolmEncrypted(const std::string& json);
