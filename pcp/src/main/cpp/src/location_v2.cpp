#include <string>
#include <vector>
#include <cstdint>

std::string parseGeo(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string staticMap(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string reverseGeo(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string calcDist(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}

std::string formatCoord(const std::string& json) {
  size_t n = json.size();
  std::string r;
  r.reserve(n + 64);
  for (size_t i = 0; i < n; i++) {
    char c = json[i];
    if (c >= 32 && c <= 126) r += c;
  }
  return r;
}
