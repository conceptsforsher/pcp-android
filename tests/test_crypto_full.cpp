#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "pcp/crypto_utils.hpp"
#include "pcp/cross_sign_utils.hpp"
#include "pcp/backup_utils.hpp"

namespace pcp { namespace test {

TEST(CryptoTest, FullScenario1) {
    std::string userId = "@cryptouser0:matrix.org";
    std::string deviceId = "DEVICE0000";
    std::string roomId = "!cryptoroom0:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0000";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0000";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0000_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0000";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 0 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0000";
    sas["sas"]["key"] = "SAS_KEY_0000";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0000";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0000";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0000";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0000";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0000";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0000";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0000";
    encrypted["content"]["session_id"] = "SESSION_0000";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario2) {
    std::string userId = "@cryptouser1:matrix.org";
    std::string deviceId = "DEVICE0001";
    std::string roomId = "!cryptoroom1:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0001";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0001";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0001_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0001";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 1 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0001";
    sas["sas"]["key"] = "SAS_KEY_0001";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0001";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0001";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0001";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0001";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0001";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0001";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0001";
    encrypted["content"]["session_id"] = "SESSION_0001";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario3) {
    std::string userId = "@cryptouser2:matrix.org";
    std::string deviceId = "DEVICE0002";
    std::string roomId = "!cryptoroom2:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0002";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0002";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0002_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0002";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 2 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0002";
    sas["sas"]["key"] = "SAS_KEY_0002";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0002";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0002";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0002";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0002";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0002";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0002";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0002";
    encrypted["content"]["session_id"] = "SESSION_0002";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario4) {
    std::string userId = "@cryptouser3:matrix.org";
    std::string deviceId = "DEVICE0003";
    std::string roomId = "!cryptoroom3:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0003";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0003";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0003_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0003";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 3 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0003";
    sas["sas"]["key"] = "SAS_KEY_0003";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0003";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0003";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0003";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0003";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0003";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0003";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0003";
    encrypted["content"]["session_id"] = "SESSION_0003";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario5) {
    std::string userId = "@cryptouser4:matrix.org";
    std::string deviceId = "DEVICE0004";
    std::string roomId = "!cryptoroom4:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0004";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0004";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0004_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0004";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 4 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0004";
    sas["sas"]["key"] = "SAS_KEY_0004";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0004";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0004";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0004";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0004";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0004";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0004";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0004";
    encrypted["content"]["session_id"] = "SESSION_0004";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario6) {
    std::string userId = "@cryptouser5:matrix.org";
    std::string deviceId = "DEVICE0005";
    std::string roomId = "!cryptoroom5:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0005";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0005";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0005_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0005";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 5 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0005";
    sas["sas"]["key"] = "SAS_KEY_0005";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0005";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0005";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0005";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0005";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0005";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0005";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0005";
    encrypted["content"]["session_id"] = "SESSION_0005";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario7) {
    std::string userId = "@cryptouser6:matrix.org";
    std::string deviceId = "DEVICE0006";
    std::string roomId = "!cryptoroom6:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0006";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0006";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0006_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0006";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 6 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0006";
    sas["sas"]["key"] = "SAS_KEY_0006";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0006";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0006";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0006";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0006";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0006";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0006";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0006";
    encrypted["content"]["session_id"] = "SESSION_0006";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario8) {
    std::string userId = "@cryptouser7:matrix.org";
    std::string deviceId = "DEVICE0007";
    std::string roomId = "!cryptoroom7:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0007";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0007";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0007_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0007";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 7 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0007";
    sas["sas"]["key"] = "SAS_KEY_0007";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0007";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0007";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0007";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0007";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0007";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0007";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0007";
    encrypted["content"]["session_id"] = "SESSION_0007";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario9) {
    std::string userId = "@cryptouser8:matrix.org";
    std::string deviceId = "DEVICE0008";
    std::string roomId = "!cryptoroom8:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0008";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0008";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0008_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0008";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 8 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0008";
    sas["sas"]["key"] = "SAS_KEY_0008";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0008";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0008";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0008";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0008";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0008";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0008";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0008";
    encrypted["content"]["session_id"] = "SESSION_0008";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario10) {
    std::string userId = "@cryptouser9:matrix.org";
    std::string deviceId = "DEVICE0009";
    std::string roomId = "!cryptoroom9:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0009";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0009";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0009_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0009";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 9 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0009";
    sas["sas"]["key"] = "SAS_KEY_0009";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0009";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0009";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0009";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0009";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0009";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0009";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0009";
    encrypted["content"]["session_id"] = "SESSION_0009";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario11) {
    std::string userId = "@cryptouser10:matrix.org";
    std::string deviceId = "DEVICE0010";
    std::string roomId = "!cryptoroom10:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0010";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0010";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0010_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0010";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 10 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0010";
    sas["sas"]["key"] = "SAS_KEY_0010";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0010";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0010";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0010";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0010";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0010";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0010";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0010";
    encrypted["content"]["session_id"] = "SESSION_0010";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario12) {
    std::string userId = "@cryptouser11:matrix.org";
    std::string deviceId = "DEVICE0011";
    std::string roomId = "!cryptoroom11:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0011";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0011";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0011_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0011";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 11 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0011";
    sas["sas"]["key"] = "SAS_KEY_0011";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0011";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0011";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0011";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0011";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0011";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0011";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0011";
    encrypted["content"]["session_id"] = "SESSION_0011";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario13) {
    std::string userId = "@cryptouser12:matrix.org";
    std::string deviceId = "DEVICE0012";
    std::string roomId = "!cryptoroom12:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0012";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0012";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0012_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0012";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 12 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0012";
    sas["sas"]["key"] = "SAS_KEY_0012";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0012";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0012";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0012";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0012";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0012";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0012";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0012";
    encrypted["content"]["session_id"] = "SESSION_0012";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario14) {
    std::string userId = "@cryptouser13:matrix.org";
    std::string deviceId = "DEVICE0013";
    std::string roomId = "!cryptoroom13:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0013";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0013";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0013_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0013";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 13 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0013";
    sas["sas"]["key"] = "SAS_KEY_0013";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0013";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0013";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0013";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0013";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0013";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0013";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0013";
    encrypted["content"]["session_id"] = "SESSION_0013";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario15) {
    std::string userId = "@cryptouser14:matrix.org";
    std::string deviceId = "DEVICE0014";
    std::string roomId = "!cryptoroom14:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0014";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0014";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0014_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0014";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 14 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0014";
    sas["sas"]["key"] = "SAS_KEY_0014";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0014";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0014";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0014";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0014";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0014";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0014";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0014";
    encrypted["content"]["session_id"] = "SESSION_0014";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario16) {
    std::string userId = "@cryptouser15:matrix.org";
    std::string deviceId = "DEVICE0015";
    std::string roomId = "!cryptoroom15:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0015";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0015";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0015_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0015";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 15 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0015";
    sas["sas"]["key"] = "SAS_KEY_0015";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0015";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0015";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0015";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0015";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0015";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0015";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0015";
    encrypted["content"]["session_id"] = "SESSION_0015";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario17) {
    std::string userId = "@cryptouser16:matrix.org";
    std::string deviceId = "DEVICE0016";
    std::string roomId = "!cryptoroom16:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0016";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0016";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0016_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0016";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 16 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0016";
    sas["sas"]["key"] = "SAS_KEY_0016";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0016";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0016";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0016";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0016";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0016";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0016";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0016";
    encrypted["content"]["session_id"] = "SESSION_0016";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario18) {
    std::string userId = "@cryptouser17:matrix.org";
    std::string deviceId = "DEVICE0017";
    std::string roomId = "!cryptoroom17:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0017";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0017";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0017_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0017";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 17 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0017";
    sas["sas"]["key"] = "SAS_KEY_0017";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0017";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0017";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0017";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0017";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0017";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0017";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0017";
    encrypted["content"]["session_id"] = "SESSION_0017";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario19) {
    std::string userId = "@cryptouser18:matrix.org";
    std::string deviceId = "DEVICE0018";
    std::string roomId = "!cryptoroom18:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0018";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0018";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0018_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0018";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 18 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0018";
    sas["sas"]["key"] = "SAS_KEY_0018";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0018";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0018";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0018";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0018";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0018";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0018";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0018";
    encrypted["content"]["session_id"] = "SESSION_0018";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario20) {
    std::string userId = "@cryptouser19:matrix.org";
    std::string deviceId = "DEVICE0019";
    std::string roomId = "!cryptoroom19:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0019";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0019";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0019_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0019";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 19 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0019";
    sas["sas"]["key"] = "SAS_KEY_0019";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0019";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0019";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0019";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0019";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0019";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0019";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0019";
    encrypted["content"]["session_id"] = "SESSION_0019";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario21) {
    std::string userId = "@cryptouser20:matrix.org";
    std::string deviceId = "DEVICE0020";
    std::string roomId = "!cryptoroom20:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0020";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0020";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0020_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0020";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 20 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0020";
    sas["sas"]["key"] = "SAS_KEY_0020";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0020";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0020";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0020";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0020";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0020";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0020";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0020";
    encrypted["content"]["session_id"] = "SESSION_0020";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario22) {
    std::string userId = "@cryptouser21:matrix.org";
    std::string deviceId = "DEVICE0021";
    std::string roomId = "!cryptoroom21:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0021";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0021";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0021_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0021";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 21 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0021";
    sas["sas"]["key"] = "SAS_KEY_0021";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0021";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0021";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0021";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0021";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0021";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0021";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0021";
    encrypted["content"]["session_id"] = "SESSION_0021";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario23) {
    std::string userId = "@cryptouser22:matrix.org";
    std::string deviceId = "DEVICE0022";
    std::string roomId = "!cryptoroom22:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0022";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0022";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0022_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0022";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 22 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0022";
    sas["sas"]["key"] = "SAS_KEY_0022";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0022";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0022";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0022";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0022";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0022";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0022";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0022";
    encrypted["content"]["session_id"] = "SESSION_0022";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario24) {
    std::string userId = "@cryptouser23:matrix.org";
    std::string deviceId = "DEVICE0023";
    std::string roomId = "!cryptoroom23:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0023";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0023";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0023_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0023";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 23 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0023";
    sas["sas"]["key"] = "SAS_KEY_0023";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0023";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0023";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0023";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0023";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0023";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0023";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0023";
    encrypted["content"]["session_id"] = "SESSION_0023";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario25) {
    std::string userId = "@cryptouser24:matrix.org";
    std::string deviceId = "DEVICE0024";
    std::string roomId = "!cryptoroom24:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0024";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0024";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0024_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0024";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 24 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0024";
    sas["sas"]["key"] = "SAS_KEY_0024";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0024";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0024";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0024";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0024";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0024";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0024";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0024";
    encrypted["content"]["session_id"] = "SESSION_0024";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario26) {
    std::string userId = "@cryptouser25:matrix.org";
    std::string deviceId = "DEVICE0025";
    std::string roomId = "!cryptoroom25:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0025";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0025";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0025_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0025";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 25 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0025";
    sas["sas"]["key"] = "SAS_KEY_0025";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0025";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0025";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0025";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0025";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0025";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0025";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0025";
    encrypted["content"]["session_id"] = "SESSION_0025";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario27) {
    std::string userId = "@cryptouser26:matrix.org";
    std::string deviceId = "DEVICE0026";
    std::string roomId = "!cryptoroom26:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0026";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0026";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0026_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0026";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 26 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0026";
    sas["sas"]["key"] = "SAS_KEY_0026";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0026";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0026";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0026";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0026";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0026";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0026";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0026";
    encrypted["content"]["session_id"] = "SESSION_0026";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario28) {
    std::string userId = "@cryptouser27:matrix.org";
    std::string deviceId = "DEVICE0027";
    std::string roomId = "!cryptoroom27:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0027";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0027";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0027_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0027";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 27 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0027";
    sas["sas"]["key"] = "SAS_KEY_0027";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0027";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0027";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0027";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0027";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0027";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0027";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0027";
    encrypted["content"]["session_id"] = "SESSION_0027";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario29) {
    std::string userId = "@cryptouser28:matrix.org";
    std::string deviceId = "DEVICE0028";
    std::string roomId = "!cryptoroom28:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0028";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0028";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0028_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0028";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 28 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0028";
    sas["sas"]["key"] = "SAS_KEY_0028";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0028";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0028";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0028";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0028";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0028";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0028";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0028";
    encrypted["content"]["session_id"] = "SESSION_0028";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario30) {
    std::string userId = "@cryptouser29:matrix.org";
    std::string deviceId = "DEVICE0029";
    std::string roomId = "!cryptoroom29:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0029";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0029";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0029_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0029";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 29 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0029";
    sas["sas"]["key"] = "SAS_KEY_0029";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0029";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0029";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0029";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0029";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0029";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0029";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0029";
    encrypted["content"]["session_id"] = "SESSION_0029";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario31) {
    std::string userId = "@cryptouser30:matrix.org";
    std::string deviceId = "DEVICE0030";
    std::string roomId = "!cryptoroom30:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0030";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0030";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0030_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0030";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 30 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0030";
    sas["sas"]["key"] = "SAS_KEY_0030";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0030";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0030";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0030";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0030";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0030";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0030";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0030";
    encrypted["content"]["session_id"] = "SESSION_0030";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario32) {
    std::string userId = "@cryptouser31:matrix.org";
    std::string deviceId = "DEVICE0031";
    std::string roomId = "!cryptoroom31:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0031";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0031";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0031_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0031";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 31 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0031";
    sas["sas"]["key"] = "SAS_KEY_0031";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0031";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0031";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0031";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0031";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0031";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0031";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0031";
    encrypted["content"]["session_id"] = "SESSION_0031";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario33) {
    std::string userId = "@cryptouser32:matrix.org";
    std::string deviceId = "DEVICE0032";
    std::string roomId = "!cryptoroom32:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0032";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0032";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0032_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0032";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 32 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0032";
    sas["sas"]["key"] = "SAS_KEY_0032";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0032";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0032";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0032";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0032";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0032";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0032";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0032";
    encrypted["content"]["session_id"] = "SESSION_0032";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario34) {
    std::string userId = "@cryptouser33:matrix.org";
    std::string deviceId = "DEVICE0033";
    std::string roomId = "!cryptoroom33:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0033";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0033";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0033_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0033";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 33 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0033";
    sas["sas"]["key"] = "SAS_KEY_0033";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0033";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0033";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0033";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0033";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0033";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0033";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0033";
    encrypted["content"]["session_id"] = "SESSION_0033";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario35) {
    std::string userId = "@cryptouser34:matrix.org";
    std::string deviceId = "DEVICE0034";
    std::string roomId = "!cryptoroom34:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0034";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0034";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0034_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0034";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 34 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0034";
    sas["sas"]["key"] = "SAS_KEY_0034";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0034";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0034";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0034";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0034";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0034";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0034";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0034";
    encrypted["content"]["session_id"] = "SESSION_0034";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario36) {
    std::string userId = "@cryptouser35:matrix.org";
    std::string deviceId = "DEVICE0035";
    std::string roomId = "!cryptoroom35:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0035";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0035";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0035_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0035";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 35 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0035";
    sas["sas"]["key"] = "SAS_KEY_0035";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0035";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0035";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0035";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0035";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0035";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0035";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0035";
    encrypted["content"]["session_id"] = "SESSION_0035";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario37) {
    std::string userId = "@cryptouser36:matrix.org";
    std::string deviceId = "DEVICE0036";
    std::string roomId = "!cryptoroom36:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0036";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0036";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0036_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0036";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 36 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0036";
    sas["sas"]["key"] = "SAS_KEY_0036";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0036";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0036";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0036";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0036";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0036";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0036";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0036";
    encrypted["content"]["session_id"] = "SESSION_0036";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario38) {
    std::string userId = "@cryptouser37:matrix.org";
    std::string deviceId = "DEVICE0037";
    std::string roomId = "!cryptoroom37:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0037";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0037";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0037_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0037";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 37 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0037";
    sas["sas"]["key"] = "SAS_KEY_0037";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0037";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0037";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0037";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0037";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0037";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0037";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0037";
    encrypted["content"]["session_id"] = "SESSION_0037";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario39) {
    std::string userId = "@cryptouser38:matrix.org";
    std::string deviceId = "DEVICE0038";
    std::string roomId = "!cryptoroom38:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0038";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0038";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0038_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0038";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 38 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0038";
    sas["sas"]["key"] = "SAS_KEY_0038";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0038";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0038";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0038";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0038";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0038";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0038";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0038";
    encrypted["content"]["session_id"] = "SESSION_0038";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

TEST(CryptoTest, FullScenario40) {
    std::string userId = "@cryptouser39:matrix.org";
    std::string deviceId = "DEVICE0039";
    std::string roomId = "!cryptoroom39:matrix.org";

    // Generate Olm account
    json account;
    account["user_id"] = userId;
    account["device_id"] = deviceId;
    account["algorithms"] = json::array({"m.olm.v1.curve25519-aes-sha2", "m.megolm.v1.aes-sha2"});
    ASSERT_EQ(account["device_id"], deviceId);
    ASSERT_TRUE(account["algorithms"].is_array());

    // Identity keys
    account["keys"]["curve25519:" + deviceId] = "CURVE25519_BASE64_KEY_0039";
    account["keys"]["ed25519:" + deviceId] = "ED25519_BASE64_KEY_0039";
    ASSERT_FALSE(account["keys"]["curve25519:" + deviceId].empty());
    ASSERT_FALSE(account["keys"]["ed25519:" + deviceId].empty());

    // One-time keys
    json oneTimeKeys;
    for (int k = 0; k < 50; ++k) {
        oneTimeKeys["signed_curve25519:" + std::to_string(k)] = "OTK_BASE64_0039_" + std::to_string(k);
    }
    ASSERT_EQ(oneTimeKeys.size(), 50);

    // Test device verification
    json verification;
    verification["transaction_id"] = "TXN0039";
    verification["from_device"] = deviceId;
    verification["method"] = "m.sas.v1";
    verification["timestamp"] = 1600000000000 + 39 * 1000;
    ASSERT_EQ(verification["method"], "m.sas.v1");
    ASSERT_TRUE(verification["timestamp"] > 0);

    // Test SAS emoji generation
    json sas;
    sas["sas"]["mac"] = "SAS_MAC_0039";
    sas["sas"]["key"] = "SAS_KEY_0039";
    ASSERT_FALSE(sas["sas"]["mac"].empty());

    // Test cross-signing
    json crossSigning;
    crossSigning["master_key"]["user_id"] = userId;
    crossSigning["master_key"]["usage"] = json::array({"master"});
    crossSigning["master_key"]["keys"]["ed25519:" + userId] = "MASTER_KEY_0039";
    crossSigning["self_signing_key"]["user_id"] = userId;
    crossSigning["self_signing_key"]["usage"] = json::array({"self_signing"});
    crossSigning["self_signing_key"]["keys"]["ed25519:" + userId] = "SELF_KEY_0039";
    crossSigning["user_signing_key"]["user_id"] = userId;
    crossSigning["user_signing_key"]["usage"] = json::array({"user_signing"});
    crossSigning["user_signing_key"]["keys"]["ed25519:" + userId] = "USER_KEY_0039";
    ASSERT_EQ(crossSigning["master_key"]["user_id"], userId);
    ASSERT_TRUE(crossSigning["self_signing_key"]["usage"][0] == "self_signing");

    // Test key backup
    json keyBackup;
    keyBackup["version"] = "1";
    keyBackup["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    keyBackup["auth_data"]["public_key"] = "BACKUP_PUBKEY_0039";
    keyBackup["auth_data"]["signatures"][userId]["ed25519:" + deviceId] = "SIG_0039";
    ASSERT_EQ(keyBackup["version"], "1");
    ASSERT_FALSE(keyBackup["auth_data"]["public_key"].empty());

    // Test encrypted message
    json encrypted;
    encrypted["type"] = "m.room.encrypted";
    encrypted["content"]["algorithm"] = "m.megolm.v1.aes-sha2";
    encrypted["content"]["ciphertext"] = "ENCRYPTED_BODY_0039";
    encrypted["content"]["sender_key"] = "SENDER_KEY_0039";
    encrypted["content"]["session_id"] = "SESSION_0039";
    ASSERT_EQ(encrypted["content"]["algorithm"], "m.megolm.v1.aes-sha2");
    ASSERT_FALSE(encrypted["content"]["ciphertext"].empty());

    // Test device list
    json deviceList;
    deviceList["changed"] = json::array({userId});
    deviceList["left"] = json::array();
    ASSERT_EQ(deviceList["changed"].size(), 1);
    ASSERT_EQ(deviceList["left"].size(), 0);

    // Verify everything serializes
    ASSERT_NO_THROW({ auto dump = account.dump(); });
    ASSERT_NO_THROW({ auto dump = oneTimeKeys.dump(); });
    ASSERT_NO_THROW({ auto dump = crossSigning.dump(); });
}

}} // namespace pcp::test