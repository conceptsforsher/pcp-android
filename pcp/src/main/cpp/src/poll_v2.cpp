#include <string>
#include <vector>
#include <cstdint>

std::string parsePoll(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string votePoll(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string calcResults(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string endPoll(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string buildRelation(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}
