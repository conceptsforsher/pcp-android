#pragma once
#include <string>
#include <vector>
#include <cstdint>

// JNI bridge stubs — Android JNI helpers
// Actual JNI functions are in app/src/main/cpp/
namespace pcp {

// Forward declarations for types used in JNI bridge
struct UserMaskRegistry { };
struct InvitationHideList { };
struct NotificationKeywords { };
struct SymbolBar { };
struct ReplacementEngine { };
struct UserMxidVisibility { };
struct LanguageHideManager { };
struct ChatPushDownManager { };
struct EmojiBlacklist { };
struct DraftManager { };
struct ScheduledEditQueue { };
struct TermsManager { };
struct UserDirectoryManager { };

struct IdentityServerManager { };
struct DeviceManager { };
struct MediaUploadManager { };
struct RoomDirectoryManager { };

// Utility function declarations referenced by jni_bridge
std::vector<std::string> parseJsonStringArray(const std::string& json);
std::string buildDirectMessageMapJson(const std::string& json);
bool isValidUserId(const std::string& userId);
std::string serverNameFromMxid(const std::string& mxid);
void addBreadcrumb(const std::string& roomId, const std::string& eventId);
std::string canonicalizeJson(const std::string& json);
std::string parseDirectMessageMap(const std::string& json);
std::string parseIgnoredUsers(const std::string& json);

struct SlidingSyncRequest { };
std::string slidingSync(const SlidingSyncRequest& req);

std::string generateDeviceName();
std::string generateDeviceId();
std::string parseWellKnown(const std::string& json);
bool isValidLoginCredentials(const std::string& user, const std::string& password);
bool needsWellKnownDiscovery(const std::string& server);
int getRetryAfterMs(const std::string& response);

std::string buildGoogleUrl(const std::string& query);
std::string buildDuckDuckGoUrl(const std::string& query);
std::string buildSearxngUrl(const std::string& query);
std::string parseOpenIdToken(const std::string& json);
std::string openIdTokenToJson(const std::string& token);
std::string parseMatrixErrorJson(const std::string& json);

void initJniBridge();
void shutdownJniBridge();

} // namespace pcp
