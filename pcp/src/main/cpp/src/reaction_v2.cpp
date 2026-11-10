#include <string>
#include <vector>
#include <cstdint>

std::string aggregateRx(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string findRx(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string buildSummary(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string processRx(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string redactRx(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}
