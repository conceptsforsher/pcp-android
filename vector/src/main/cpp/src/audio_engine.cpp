#include "progressive/audio_engine.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <unordered_map>

namespace progressive {

// ============================================================
// EXISTING APIS — format utilities
// ============================================================

std::string formatDuration(int64_t ms) {
    if (ms < 0) ms = 0;
    int64_t seconds = ms / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;
    std::ostringstream out;
    if (hours > 0) {
        out << hours << ":" << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;
    } else {
        out << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    }
    return out.str();
}

std::string formatPositionInfo(int64_t positionMs, int64_t durationMs) {
    return formatDuration(positionMs) + " / " + formatDuration(durationMs);
}

float computeProgress(int64_t positionMs, int64_t durationMs) {
    if (durationMs <= 0) return 0.0f;
    float p = static_cast<float>(positionMs) / static_cast<float>(durationMs);
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    return p;
}

bool isSupportedAudioType(const std::string& mimeType) {
    static const char* supported[] = {
        "audio/mpeg", "audio/mp3", "audio/mp4", "audio/m4a",
        "audio/ogg", "audio/opus", "audio/wav", "audio/wave",
        "audio/x-wav", "audio/flac", "audio/aac", "audio/webm",
        "audio/3gpp", "audio/x-ms-wma", "audio/amr"
    };
    for (const auto& s : supported) if (mimeType == s) return true;
    return false;
}

std::string mimeToExtension(const std::string& mimeType) {
    if (mimeType == "audio/mpeg" || mimeType == "audio/mp3") return "mp3";
    if (mimeType == "audio/mp4" || mimeType == "audio/m4a") return "m4a";
    if (mimeType == "audio/ogg") return "ogg";
    if (mimeType == "audio/opus") return "opus";
    if (mimeType == "audio/wav" || mimeType == "audio/wave" || mimeType == "audio/x-wav") return "wav";
    if (mimeType == "audio/flac") return "flac";
    if (mimeType == "audio/aac") return "aac";
    if (mimeType == "audio/webm") return "webm";
    if (mimeType == "audio/3gpp") return "3gp";
    if (mimeType == "audio/amr") return "amr";
    return "";
}

// ============================================================
// AUDIO FORMAT DETECTION
// Original Kotlin:
//   Audio format detection from file headers / magic bytes
// ============================================================

AudioFormat detectAudioFormat(const std::string& filenameOrPath) {
    // Original Kotlin:
    //   Detect format from filename extension
    auto dot = filenameOrPath.rfind('.');
    if (dot == std::string::npos) return AudioFormat::UNKNOWN;

    std::string ext = filenameOrPath.substr(dot + 1);
    for (char& c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (ext == "mp3") return AudioFormat::MP3;
    if (ext == "aac") return AudioFormat::AAC;
    if (ext == "ogg") return AudioFormat::OGG_VORBIS;
    if (ext == "opus") return AudioFormat::OPUS;
    if (ext == "flac") return AudioFormat::FLAC;
    if (ext == "wav" || ext == "wave") return AudioFormat::WAV;
    if (ext == "wma") return AudioFormat::WMA;
    if (ext == "m4a" || ext == "mp4") return AudioFormat::M4A;
    if (ext == "amr") return AudioFormat::AMR;
    return AudioFormat::UNKNOWN;
}

AudioFormat detectAudioFormat(const std::vector<uint8_t>& headerBytes) {
    // Original Kotlin:
    //   Detect format from file magic bytes (first few bytes of file)
    if (headerBytes.size() < 4) return AudioFormat::UNKNOWN;

    // MP3: ID3 tag "ID3" (bytes 0-2) or sync word 0xFF 0xFB/0xFA/0xF3/0xF2
    if (headerBytes.size() >= 3) {
        if (headerBytes[0] == 'I' && headerBytes[1] == 'D' && headerBytes[2] == '3')
            return AudioFormat::MP3;
    }
    if (headerBytes.size() >= 2) {
        if ((headerBytes[0] == 0xFF && (headerBytes[1] & 0xE0) == 0xE0))
            return AudioFormat::MP3;
    }

    // WAV: "RIFF" + "WAVE"
    if (headerBytes.size() >= 12) {
        if (memcmp(headerBytes.data(), "RIFF", 4) == 0 &&
            memcmp(headerBytes.data() + 8, "WAVE", 4) == 0)
            return AudioFormat::WAV;
    }

    // FLAC: "fLaC" at byte 0
    if (headerBytes.size() >= 4) {
        if (memcmp(headerBytes.data(), "fLaC", 4) == 0)
            return AudioFormat::FLAC;
    }

    // OGG: "OggS" at byte 0
    if (headerBytes.size() >= 4) {
        if (memcmp(headerBytes.data(), "OggS", 4) == 0) {
            // Could be Vorbis or Opus; check secondary header
            if (headerBytes.size() >= 36) {
                // OpusHead starts at offset 28 in Ogg page
                if (memcmp(headerBytes.data() + 28, "OpusHead", 8) == 0)
                    return AudioFormat::OPUS;
                // Vorbis: packet type 0x01 at offset 28 in first vorbis packet
                if (headerBytes[28] == 0x01)
                    return AudioFormat::OGG_VORBIS;
            }
            return AudioFormat::OGG_VORBIS; // default to Vorbis for OGG
        }
    }

    // AAC: ADTS header 0xFFF (sync word: 0xFFF)
    if (headerBytes.size() >= 2) {
        if (headerBytes[0] == 0xFF && (headerBytes[1] & 0xF0) == 0xF0)
            return AudioFormat::AAC;
    }

    // WMA: ASF header "30 26 B2 75 8E 66 CF 11 A6 D9 00 AA 00 62 CE 6C"
    if (headerBytes.size() >= 16) {
        static const uint8_t asfGuid[16] = {
            0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
            0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C
        };
        if (memcmp(headerBytes.data(), asfGuid, 16) == 0)
            return AudioFormat::WMA;
    }

    // M4A/MP4: "ftyp" at offset 4
    if (headerBytes.size() >= 12) {
        if (memcmp(headerBytes.data() + 4, "ftyp", 4) == 0) {
            // Check for "M4A " subtype
            if (headerBytes.size() >= 16 &&
                memcmp(headerBytes.data() + 8, "M4A ", 4) == 0)
                return AudioFormat::M4A;
            return AudioFormat::M4A; // generic MP4 container
        }
    }

    // AMR: "#!AMR\n" or "#!AMR-WB\n"
    if (headerBytes.size() >= 6) {
        if (memcmp(headerBytes.data(), "#!AMR\n", 6) == 0)
            return AudioFormat::AMR;
        if (headerBytes.size() >= 9 &&
            memcmp(headerBytes.data(), "#!AMR-WB\n", 9) == 0)
            return AudioFormat::AMR;
    }

    return AudioFormat::UNKNOWN;
}

std::string audioFormatToString(AudioFormat fmt) {
    // Original Kotlin:
    //   enum name conversion
    switch (fmt) {
        case AudioFormat::MP3:        return "MP3";
        case AudioFormat::AAC:        return "AAC";
        case AudioFormat::OGG_VORBIS: return "OGG Vorbis";
        case AudioFormat::OPUS:       return "Opus";
        case AudioFormat::FLAC:       return "FLAC";
        case AudioFormat::WAV:        return "WAV";
        case AudioFormat::WMA:        return "WMA";
        case AudioFormat::M4A:        return "M4A";
        case AudioFormat::AMR:        return "AMR";
        default:                      return "Unknown";
    }
}

std::string audioFormatToMime(AudioFormat fmt) {
    switch (fmt) {
        case AudioFormat::MP3:        return "audio/mpeg";
        case AudioFormat::AAC:        return "audio/aac";
        case AudioFormat::OGG_VORBIS: return "audio/ogg";
        case AudioFormat::OPUS:       return "audio/opus";
        case AudioFormat::FLAC:       return "audio/flac";
        case AudioFormat::WAV:        return "audio/wav";
        case AudioFormat::WMA:        return "audio/x-ms-wma";
        case AudioFormat::M4A:        return "audio/mp4";
        case AudioFormat::AMR:        return "audio/amr";
        default:                      return "application/octet-stream";
    }
}

// ============================================================
// AUDIO METADATA PARSING
// Original Kotlin:
//   ID3 tag parser + basic container metadata extraction
// ============================================================

// Parse ID3v1 tag (last 128 bytes of MP3)
static void parseId3v1(const uint8_t* data, size_t size, AudioMetadata& meta) {
    if (size < 128) return;
    const uint8_t* tag = data + size - 128;
    if (tag[0] != 'T' || tag[1] != 'A' || tag[2] != 'G') return;

    auto copyField = [&](size_t offset, size_t length, std::string& dest) {
        dest.assign(reinterpret_cast<const char*>(tag + offset), length);
        while (!dest.empty() && dest.back() == 0) dest.pop_back();
        while (!dest.empty() && dest.back() == ' ') dest.pop_back();
    };

    copyField(3, 30, meta.title);
    copyField(33, 30, meta.artist);
    copyField(63, 30, meta.album);
    std::string yearStr;
    copyField(93, 4, yearStr);
    if (!yearStr.empty()) meta.year = std::stoi(yearStr);

    // Track number: offset 126, but only in ID3v1.1 (if byte 125 is 0)
    if (tag[125] == 0 && tag[126] != 0) meta.trackNumber = tag[126];

    // Genre: offset 127
    meta.genre = std::to_string(tag[127]); // genre code for now
    meta.valid = true;
}

// Parse ID3v2 tag (variable-length header at start of MP3)
static size_t parseId3v2(const uint8_t* data, size_t size, AudioMetadata& meta) {
    if (size < 10) return 0;
    if (data[0] != 'I' || data[1] != 'D' || data[2] != '3') return 0;

    uint8_t majorVer = data[3];
    uint8_t minorVer = data[4];
    uint8_t flags = data[5];

    // Tag size: 4 bytes, sync-safe integer (bit 7 always 0 in each byte)
    uint32_t tagSize = (static_cast<uint32_t>(data[6] & 0x7F) << 21) |
                       (static_cast<uint32_t>(data[7] & 0x7F) << 14) |
                       (static_cast<uint32_t>(data[8] & 0x7F) << 7) |
                        static_cast<uint32_t>(data[9] & 0x7F);
    tagSize += 10; // header size

    (void)majorVer; (void)minorVer; (void)flags;
    // Simplified: we don't parse full ID3v2 frames, just skip past the tag
    return std::min(tagSize, static_cast<uint32_t>(size));
}

// Parse FLAC metadata blocks
static void parseFlacMetadata(const uint8_t* data, size_t size, AudioMetadata& meta) {
    if (size < 42) return; // fLaC + STREAMINFO (34 bytes) + min
    // FLAC stream marker already checked by detectAudioFormat
    const uint8_t* p = data + 4; // skip "fLaC"

    bool lastBlock = false;
    while (!lastBlock && (p - data) + 4 <= static_cast<ptrdiff_t>(size)) {
        lastBlock = (p[0] & 0x80) != 0;
        uint32_t blockType = p[0] & 0x7F;
        uint32_t blockSize = (static_cast<uint32_t>(p[1]) << 16) |
                             (static_cast<uint32_t>(p[2]) << 8) | p[3];

        if (blockType == 0) {
            // STREAMINFO block: minimum/maximum block size, sample rate, channels, etc.
            if (blockSize >= 18 && (p - data) + 4 + 18 <= static_cast<ptrdiff_t>(size)) {
                const uint8_t* info = p + 4;
                meta.sampleRate = ((info[10] << 12) | (info[11] << 4) | (info[12] >> 4));
                meta.channels = ((info[12] & 0x0E) >> 1) + 1;
                meta.bitRate = (meta.sampleRate * 16 * meta.channels) / 1000;
                meta.codec = "flac";
                // Duration from total samples
                uint64_t totalSamples = (static_cast<uint64_t>(info[13] & 0x0F) << 32) |
                                        (static_cast<uint64_t>(info[14]) << 24) |
                                        (static_cast<uint64_t>(info[15]) << 16) |
                                        (static_cast<uint64_t>(info[16]) << 8) | info[17];
                if (meta.sampleRate > 0)
                    meta.durationMs = static_cast<int64_t>((totalSamples * 1000) / meta.sampleRate);
                meta.valid = true;
            }
        } else if (blockType == 4) {
            // VORBIS_COMMENT: can contain title, artist, album
            if (blockSize >= 4 && (p - data) + 4 + static_cast<ptrdiff_t>(blockSize) <= static_cast<ptrdiff_t>(size)) {
                const uint8_t* vc = p + 4;
                uint32_t vendorLen = vc[0] | (vc[1] << 8) | (vc[2] << 16) | (vc[3] << 24);
                vc += 4 + vendorLen;
                if (vc < p + 4 + blockSize) {
                    uint32_t numComments = vc[0] | (vc[1] << 8) | (vc[2] << 16) | (vc[3] << 24);
                    vc += 4;
                    for (uint32_t i = 0; i < numComments && vc < p + 4 + blockSize; ++i) {
                        if (vc + 4 > p + 4 + blockSize) break;
                        uint32_t len = vc[0] | (vc[1] << 8) | (vc[2] << 16) | (vc[3] << 24);
                        vc += 4;
                        if (vc + len > p + 4 + blockSize) break;
                        std::string comment(reinterpret_cast<const char*>(vc), len);
                        vc += len;
                        auto eq = comment.find('=');
                        if (eq != std::string::npos) {
                            std::string key = comment.substr(0, eq);
                            std::string val = comment.substr(eq + 1);
                            for (char& c : key) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                            if (key == "TITLE") meta.title = val;
                            else if (key == "ARTIST") meta.artist = val;
                            else if (key == "ALBUM") meta.album = val;
                            else if (key == "GENRE") meta.genre = val;
                            else if (key == "DATE") { try { meta.year = std::stoi(val); } catch (...) {} }
                            else if (key == "TRACKNUMBER") { try { meta.trackNumber = std::stoi(val); } catch (...) {} }
                        }
                    }
                }
            }
        }

        p += 4 + blockSize;
    }
}

// WAV header parser
static void parseWavMetadata(const uint8_t* data, size_t size, AudioMetadata& meta) {
    if (size < 44) return;
    // "RIFF" + size + "WAVE" + "fmt " + subchunk1size
    uint16_t audioFormat = data[20] | (data[21] << 8);
    meta.channels = data[22] | (data[23] << 8);
    meta.sampleRate = data[24] | (data[25] << 8) | (data[26] << 16) | (data[27] << 24);
    uint16_t bitsPerSample = data[34] | (data[35] << 8);

    if (meta.sampleRate > 0 && meta.channels > 0) {
        meta.bitRate = (meta.sampleRate * meta.channels * bitsPerSample) / 1000;
    }
    meta.codec = "pcm";
    meta.valid = true;

    // Calculate duration from data size
    uint32_t dataSize = data[40] | (data[41] << 8) | (data[42] << 16) | (data[43] << 24);
    if (meta.sampleRate > 0 && meta.channels > 0 && bitsPerSample > 0) {
        size_t bytesPerSec = static_cast<size_t>(meta.sampleRate) * meta.channels * (bitsPerSample / 8);
        if (bytesPerSec > 0) {
            meta.durationMs = static_cast<int64_t>(dataSize) * 1000 / static_cast<int64_t>(bytesPerSec);
        }
    }
    (void)audioFormat;
}

AudioMetadata parseAudioMetadata(const std::vector<uint8_t>& rawBytes) {
    // Original Kotlin:
    //   Combined metadata extraction from raw file bytes
    AudioMetadata meta;
    if (rawBytes.empty()) return meta;

    meta.fileSize = static_cast<int64_t>(rawBytes.size());
    meta.format = detectAudioFormat(rawBytes);

    switch (meta.format) {
        case AudioFormat::MP3:
            parseId3v2(rawBytes.data(), rawBytes.size(), meta);
            parseId3v1(rawBytes.data(), rawBytes.size(), meta);
            meta.codec = "mp3";
            break;
        case AudioFormat::FLAC:
            parseFlacMetadata(rawBytes.data(), rawBytes.size(), meta);
            break;
        case AudioFormat::WAV:
            parseWavMetadata(rawBytes.data(), rawBytes.size(), meta);
            break;
        case AudioFormat::OGG_VORBIS:
        case AudioFormat::OPUS:
            meta.codec = (meta.format == AudioFormat::OPUS) ? "opus" : "vorbis";
            // Ogg container: Vorbis/Opus comments similar to FLAC Vorbis comments
            parseFlacMetadata(rawBytes.data(), rawBytes.size(), meta);
            break;
        case AudioFormat::M4A:
        case AudioFormat::AAC:
            meta.codec = "aac";
            break;
        default:
            break;
    }
    return meta;
}

AudioMetadata parseAudioMetadata(const std::string& json) {
    // Original Kotlin:
    //   Parse JSON object {"title": "...", "artist": "...", ...}
    AudioMetadata meta;
    if (json.empty()) return meta;

    auto getField = [&json](const std::string& key) -> std::string {
        std::string searchKey = "\"" + key + "\"";
        auto pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        pos = json.find(':', pos + searchKey.size());
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        if (pos >= json.size()) return "";
        if (json[pos] == '"') {
            pos++;
            auto end = json.find('"', pos);
            if (end == std::string::npos) return "";
            return json.substr(pos, end - pos);
        } else {
            auto end = json.find_first_of(",}\n\r", pos);
            std::string val = json.substr(pos, end - pos);
            while (!val.empty() && (val.back() == ' ' || val.back() == '\t')) val.pop_back();
            return val;
        }
    };

    meta.title = getField("title");
    meta.artist = getField("artist");
    meta.album = getField("album");
    meta.genre = getField("genre");
    meta.codec = getField("codec");
    std::string yearStr = getField("year");
    try { if (!yearStr.empty()) meta.year = std::stoi(yearStr); } catch (...) {}
    std::string trackStr = getField("trackNumber");
    try { if (!trackStr.empty()) meta.trackNumber = std::stoi(trackStr); } catch (...) {}
    std::string durStr = getField("durationMs");
    try { if (!durStr.empty()) meta.durationMs = std::stoll(durStr); } catch (...) {}
    std::string brStr = getField("bitRate");
    try { if (!brStr.empty()) meta.bitRate = std::stoi(brStr); } catch (...) {}
    std::string srStr = getField("sampleRate");
    try { if (!srStr.empty()) meta.sampleRate = std::stoi(srStr); } catch (...) {}
    std::string chStr = getField("channels");
    try { if (!chStr.empty()) meta.channels = std::stoi(chStr); } catch (...) {}
    std::string fsStr = getField("fileSize");
    try { if (!fsStr.empty()) meta.fileSize = std::stoll(fsStr); } catch (...) {}

    meta.valid = !meta.title.empty() || !meta.artist.empty() || meta.durationMs > 0;
    return meta;
}

// ============================================================
// AUDIO WAVEFORM GENERATION
// Original Kotlin:
//   Visual waveform from PCM audio data for voice message display
// ============================================================

AudioWaveform generateWaveform(const std::vector<int16_t>& pcmSamples,
                                 int sampleRate, int64_t durationMs,
                                 int targetNumSamples) {
    AudioWaveform result;
    result.sampleRate = sampleRate;
    result.durationMs = durationMs;
    if (pcmSamples.empty() || targetNumSamples <= 0) return result;

    // Divide audio into buckets
    size_t totalSamples = pcmSamples.size();
    int numBins = std::min(targetNumSamples, static_cast<int>(totalSamples));
    if (numBins <= 0) return result;

    result.numSamples = numBins;
    result.samples.resize(numBins);

    size_t samplesPerBin = totalSamples / numBins;
    if (samplesPerBin == 0) samplesPerBin = 1;

    float maxAmp = 0.0f;
    const float normFactor = 1.0f / 32768.0f; // int16_t max

    for (int i = 0; i < numBins; ++i) {
        size_t start = i * samplesPerBin;
        size_t end = (i == numBins - 1) ? totalSamples : start + samplesPerBin;
        float sumSq = 0.0f;
        for (size_t j = start; j < end; ++j) {
            float sample = static_cast<float>(pcmSamples[j]) * normFactor;
            sumSq += sample * sample;
        }
        float rms = std::sqrt(sumSq / static_cast<float>(end - start));
        result.samples[i] = rms;
        if (rms > maxAmp) maxAmp = rms;
    }

    result.maxAmplitude = maxAmp;
    return result;
}

AudioWaveform scaleWaveform(const AudioWaveform& waveform, int targetNumSamples) {
    // Original Kotlin:
    //   Scale/output a waveform to a target number of visual sample points
    if (targetNumSamples <= 0 || waveform.samples.empty()) return waveform;
    if (waveform.numSamples == targetNumSamples) return waveform;

    AudioWaveform result;
    result.sampleRate = waveform.sampleRate;
    result.durationMs = waveform.durationMs;
    result.numSamples = targetNumSamples;
    result.samples.resize(targetNumSamples);

    float ratio = static_cast<float>(waveform.numSamples) / static_cast<float>(targetNumSamples);
    float maxAmp = 0.0f;

    for (int i = 0; i < targetNumSamples; ++i) {
        float srcPos = static_cast<float>(i) * ratio;
        int lo = static_cast<int>(std::floor(srcPos));
        int hi = static_cast<int>(std::ceil(srcPos));
        if (hi >= waveform.numSamples) hi = waveform.numSamples - 1;
        if (lo >= waveform.numSamples) lo = waveform.numSamples - 1;

        float frac = srcPos - static_cast<float>(lo);
        float val = waveform.samples[lo] * (1.0f - frac) + waveform.samples[hi] * frac;
        result.samples[i] = val;
        if (val > maxAmp) maxAmp = val;
    }

    result.maxAmplitude = maxAmp;
    return result;
}

// ============================================================
// AUDIO PLAYER / RECORDER HELPERS
// ============================================================

std::string formatAudioDuration(int64_t ms) {
    return formatDuration(ms);
}

std::string formatAudioPositionInfo(int64_t positionMs, int64_t durationMs) {
    return formatPositionInfo(positionMs, durationMs);
}

bool isAudioVoiceMessage(const std::string& mimeType) {
    // Original Kotlin:
    //   Check if this is an audio/voice message (not music)
    // Voice messages are typically: audio/ogg, audio/opus, audio/aac, audio/mp4 (m4a), audio/amr
    return mimeType == "audio/ogg" || mimeType == "audio/opus" ||
           mimeType == "audio/aac" || mimeType == "audio/mp4" ||
           mimeType == "audio/m4a" || mimeType == "audio/amr" ||
           mimeType == "audio/3gpp";
}

AudioPlayerInfo buildAudioInfo(const std::string& json) {
    // Original Kotlin:
    //   Parse AudioPlayerInfo from JSON
    AudioPlayerInfo info;
    if (json.empty()) return info;

    auto getInt = [&json](const std::string& key) -> int64_t {
        std::string searchKey = "\"" + key + "\"";
        auto pos = json.find(searchKey);
        if (pos == std::string::npos) return 0;
        pos = json.find(':', pos + searchKey.size());
        if (pos == std::string::npos) return 0;
        pos++;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        auto end = json.find_first_of(",}\n\r", pos);
        std::string val = json.substr(pos, end - pos);
        try { return std::stoll(val); } catch (...) { return 0; }
    };
    auto getFloat = [&json](const std::string& key) -> float {
        std::string searchKey = "\"" + key + "\"";
        auto pos = json.find(searchKey);
        if (pos == std::string::npos) return 0.0f;
        pos = json.find(':', pos + searchKey.size());
        if (pos == std::string::npos) return 0.0f;
        pos++;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        auto end = json.find_first_of(",}\n\r", pos);
        std::string val = json.substr(pos, end - pos);
        try { return std::stof(val); } catch (...) { return 0.0f; }
    };
    auto getBool = [&json](const std::string& key) -> bool {
        std::string searchKey = "\"" + key + "\"";
        auto pos = json.find(searchKey);
        if (pos == std::string::npos) return false;
        pos = json.find(':', pos + searchKey.size());
        if (pos == std::string::npos) return false;
        pos++;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        return pos < json.size() && json[pos] == 't'; // true/false
    };
    auto getStr = [&json](const std::string& key) -> std::string {
        std::string searchKey = "\"" + key + "\"";
        auto pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        pos = json.find(':', pos + searchKey.size());
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        if (pos >= json.size() || json[pos] != '"') return "";
        pos++;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    info.currentPositionMs = getInt("currentPositionMs");
    info.durationMs = getInt("durationMs");
    info.isLooping = getBool("isLooping");
    info.volume = getFloat("volume");
    info.playbackSpeed = getFloat("playbackSpeed");
    info.errorMessage = getStr("errorMessage");
    std::string stateStr = getStr("state");
    if (stateStr == "PLAYING") info.state = AudioPlayerState::PLAYING;
    else if (stateStr == "PAUSED") info.state = AudioPlayerState::PAUSED;
    else if (stateStr == "BUFFERING") info.state = AudioPlayerState::BUFFERING;
    else if (stateStr == "ERROR") info.state = AudioPlayerState::ERROR;
    else info.state = AudioPlayerState::STOPPED;

    return info;
}

std::string parseAudioInfo(const AudioPlayerInfo& info) {
    // Original Kotlin:
    //   Serialize AudioPlayerInfo to JSON string
    std::ostringstream json;
    json << "{";
    json << "\"state\": \"";
    switch (info.state) {
        case AudioPlayerState::STOPPED:    json << "STOPPED";    break;
        case AudioPlayerState::PLAYING:    json << "PLAYING";    break;
        case AudioPlayerState::PAUSED:     json << "PAUSED";     break;
        case AudioPlayerState::BUFFERING:  json << "BUFFERING";  break;
        case AudioPlayerState::ERROR:      json << "ERROR";      break;
    }
    json << "\",";
    json << "\"currentPositionMs\": " << info.currentPositionMs << ",";
    json << "\"durationMs\": " << info.durationMs << ",";
    json << "\"isLooping\": " << (info.isLooping ? "true" : "false") << ",";
    json << "\"volume\": " << std::fixed << std::setprecision(2) << info.volume << ",";
    json << "\"playbackSpeed\": " << std::fixed << std::setprecision(2) << info.playbackSpeed;
    if (!info.errorMessage.empty()) {
        json << ",\"errorMessage\": \"" << info.errorMessage << "\"";
    }
    json << "}";
    return json.str();
}

// ============================================================
// PLAYBACK SPEED
// ============================================================

float playbackSpeedValue(AudioPlaybackSpeed speed) {
    // Original Kotlin:
    //   when(speed) { HALF -> 0.5f; NORMAL -> 1.0f; ONE_POINT_FIVE -> 1.5f; DOUBLE -> 2.0f }
    switch (speed) {
        case AudioPlaybackSpeed::HALF:           return 0.5f;
        case AudioPlaybackSpeed::NORMAL:         return 1.0f;
        case AudioPlaybackSpeed::ONE_POINT_FIVE: return 1.5f;
        case AudioPlaybackSpeed::DOUBLE:         return 2.0f;
    }
    return 1.0f;
}

std::string playbackSpeedLabel(AudioPlaybackSpeed speed) {
    switch (speed) {
        case AudioPlaybackSpeed::HALF:           return "0.5x";
        case AudioPlaybackSpeed::NORMAL:         return "1x";
        case AudioPlaybackSpeed::ONE_POINT_FIVE: return "1.5x";
        case AudioPlaybackSpeed::DOUBLE:         return "2x";
    }
    return "1x";
}

// ============================================================
// AUDIO VISUALIZER — frequency domain and time domain analysis
// Original Kotlin:
//   FFT-based frequency visualization for voice messages
// ============================================================

namespace AudioVisualizer {

std::vector<float> computeFrequencyData(const std::vector<int16_t>& pcmSamples,
                                          int sampleRate, int numBins) {
    // Original Kotlin:
    //   Compute DFT magnitudes for numBins frequency bins
    std::vector<float> magnitudes(numBins, 0.0f);
    if (pcmSamples.empty() || numBins <= 0 || sampleRate <= 0) return magnitudes;

    // Use a windowed DFT approach with Hann window
    size_t N = pcmSamples.size();
    float normFactor = 1.0f / 32768.0f;

    // Hann window
    std::vector<float> window(N);
    for (size_t i = 0; i < N; ++i) {
        window[i] = 0.5f * (1.0f - std::cos(2.0f * M_PI * static_cast<float>(i) / static_cast<float>(N - 1)));
    }

    // Compute magnitudes for target frequency bins
    // Map bin index to frequency bin linearly from 0 to Nyquist
    std::vector<float> windowedSamples(N);
    for (size_t i = 0; i < N; ++i) {
        windowedSamples[i] = window[i] * static_cast<float>(pcmSamples[i]) * normFactor;
    }

    for (int bin = 0; bin < numBins; ++bin) {
        // Map bin index to frequency
        float freq = static_cast<float>(bin) * static_cast<float>(sampleRate) / 2.0f / static_cast<float>(numBins);
        if (freq <= 0.0f) { magnitudes[bin] = 0.0f; continue; }

        // Goertzel algorithm for single DFT bin
        float omega = 2.0f * static_cast<float>(M_PI) * freq / static_cast<float>(sampleRate);
        float coeff = 2.0f * std::cos(omega);
        float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f;

        for (size_t i = 0; i < N; ++i) {
            s0 = windowedSamples[i] + coeff * s1 - s2;
            s2 = s1;
            s1 = s0;
        }

        float real = s1 - s2 * std::cos(omega);
        float imag = s2 * std::sin(omega);
        magnitudes[bin] = std::sqrt(real * real + imag * imag) / static_cast<float>(N);
    }

    // Normalize
    float maxMag = 0.0f;
    for (float& m : magnitudes) if (m > maxMag) maxMag = m;
    if (maxMag > 0.0f) {
        for (float& m : magnitudes) m /= maxMag;
    }

    return magnitudes;
}

std::vector<float> computeTimeData(const std::vector<int16_t>& pcmSamples, int numPoints) {
    // Original Kotlin:
    //   Compute RMS envelope for time-domain waveform display
    std::vector<float> envelope(numPoints, 0.0f);
    if (pcmSamples.empty() || numPoints <= 0) return envelope;

    size_t N = pcmSamples.size();
    float normFactor = 1.0f / 32768.0f;

    // Compute RMS in each time bucket
    size_t samplesPerPoint = N / static_cast<size_t>(numPoints);
    if (samplesPerPoint == 0) samplesPerPoint = 1;

    float maxVal = 0.0f;
    for (int point = 0; point < numPoints; ++point) {
        size_t start = static_cast<size_t>(point) * samplesPerPoint;
        size_t end = std::min(start + samplesPerPoint, N);
        float sumSq = 0.0f;
        for (size_t i = start; i < end; ++i) {
            float sample = static_cast<float>(pcmSamples[i]) * normFactor;
            sumSq += sample * sample;
        }
        float rms = std::sqrt(sumSq / static_cast<float>(end - start));
        envelope[point] = rms;
        if (rms > maxVal) maxVal = rms;
    }

    // Normalize
    if (maxVal > 0.0f) {
        for (float& v : envelope) v /= maxVal;
    }

    return envelope;
}

} // namespace AudioVisualizer

} // namespace progressive
