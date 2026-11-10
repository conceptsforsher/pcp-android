#include <string>
#include <vector>
#include <cstdint>

std::string processPush(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string filterRoom(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string decryptPush(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string formatTitle(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string buildIntent(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}
