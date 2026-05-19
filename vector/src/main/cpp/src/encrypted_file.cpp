#include "progressive/encrypted_file.hpp"
#include <sstream>

namespace progressive {

// ==== EncryptedFileKey Validation (from EncryptedFileKey.kt:57-79) ====
bool EncryptedFileKey::isValid() const {
    if (alg != "A256CTR") return false;
    if (!ext) return false;
    bool hasEncrypt = false, hasDecrypt = false;
    for (const auto& op : keyOps) {
        if (op == "encrypt") hasEncrypt = true;
        if (op == "decrypt") hasDecrypt = true;
    }
    if (!hasEncrypt || !hasDecrypt) return false;
    if (kty != "oct") return false;
    if (k.empty()) return false;
    return true;
}

bool EncFileInfo::isValid() const {
    if (url.empty()) return false;
    if (!key.isValid()) return false;
    if (iv.empty()) return false;
    if (hashes.find("sha256") == hashes.end()) return false;
    if (version != "v2") return false;
    return true;
}

// ==== JSON Parsers ====

EncryptedFileKey parseEncryptedFileKey(const std::string& json) {
    EncryptedFileKey key;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    key.alg = extractStr("alg");
    key.kty = extractStr("kty");
    key.k = extractStr("k");
    key.ext = json.find("\"ext\": true") != std::string::npos || json.find("\"ext\":true") != std::string::npos;

    // Parse key_ops array
    auto opsPos = json.find("\"key_ops\"");
    if (opsPos != std::string::npos) {
        auto bracket = json.find('[', opsPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size()) {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        key.keyOps.push_back(json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                if (json[pos] == ']') break;
                pos++;
            }
        }
    }

    key.valid = key.isValid();
    return key;
}

EncFileInfo parseEncryptedFileInfo(const std::string& json) {
    EncFileInfo info;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    info.url = extractStr("url");
    info.iv = extractStr("iv");
    info.version = extractStr("v");

    // Parse nested key object
    auto keyPos = json.find("\"key\"");
    if (keyPos != std::string::npos) {
        auto brace = json.find('{', keyPos);
        if (brace != std::string::npos) {
            int depth = 1;
            size_t end = brace + 1;
            while (end < json.size() && depth > 0) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') depth--;
                end++;
            }
            info.key = parseEncryptedFileKey(json.substr(brace, end - brace));
        }
    }

    // Parse hashes map
    auto hashesPos = json.find("\"hashes\"");
    if (hashesPos != std::string::npos) {
        auto brace = json.find('{', hashesPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string hashKey = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto valQuote = json.find('"', colon);
                    if (valQuote == std::string::npos) break;
                    auto valEnd = json.find('"', valQuote + 1);
                    if (valEnd == std::string::npos) break;
                    info.hashes[hashKey] = json.substr(valQuote + 1, valEnd - valQuote - 1);
                    pos = valEnd + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    info.valid = info.isValid();
    return info;
}

bool isValidJwkKey(const EncryptedFileKey& key) { return key.isValid(); }
bool isValidEncryptedFile(const EncFileInfo& info) { return info.isValid(); }

std::string extractFileKey(const EncryptedFileKey& key) { return key.k; }
std::string extractFileIv(const EncFileInfo& info) { return info.iv; }

std::string encryptedFileKeyToJson(const EncryptedFileKey& key) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (key.isValid() ? "true" : "false") << ",";
    json << R"("alg": ")" << esc(key.alg) << R"(",)";
    json << R"("kty": ")" << esc(key.kty) << R"(",)";
    json << R"("ext": )" << (key.ext ? "true" : "false");
    json << "}";
    return json.str();
}

std::string encryptedFileInfoToJson(const EncFileInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (info.isValid() ? "true" : "false") << ",";
    json << R"("url": ")" << esc(info.url) << R"(",)";
    json << R"("version": ")" << esc(info.version) << R"(",)";
    json << R"("iv": ")" << esc(info.iv) << R"(")";
    json << "}";
    return json.str();
}

} // namespace progressive
