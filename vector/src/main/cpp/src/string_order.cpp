#include "progressive/string_order.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <cstring>
#include <unordered_set>
#include <stdexcept>

namespace progressive {

// ==== Simple Big Integer Helpers (decimal strings) ====
// Original Kotlin:
//   BigInteger emulation for fractional indexing

static std::string addBigInt(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0;
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result += static_cast<char>('0' + (sum % 10));
        carry = sum / 10;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

static std::string subBigInt(const std::string& a, const std::string& b) {
    std::string result;
    int borrow = 0;
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    while (i >= 0) {
        int diff = (a[i--] - '0') - borrow;
        if (j >= 0) diff -= (b[j--] - '0');
        if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
        result += static_cast<char>('0' + diff);
    }
    std::reverse(result.begin(), result.end());
    while (result.size() > 1 && result[0] == '0') result.erase(0, 1);
    return result;
}

static std::string mulBigInt(const std::string& a, const std::string& b) {
    std::string result(a.size() + b.size(), '0');
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = static_cast<int>(b.size()) - 1; j >= 0; --j) {
            int sum = (a[i] - '0') * (b[j] - '0') + (result[i + j + 1] - '0') + carry;
            result[i + j + 1] = '0' + (sum % 10);
            carry = sum / 10;
        }
        result[i] += carry;
    }
    while (result.size() > 1 && result[0] == '0') result.erase(0, 1);
    return result;
}

static std::string divBigInt(const std::string& a, const std::string& b) {
    if (a.size() < b.size() || (a.size() == b.size() && a < b)) return "0";
    std::string result;
    std::string current;
    for (char c : a) {
        current += c;
        while (current.size() > 1 && current[0] == '0') current.erase(0, 1);
        int q = 0;
        std::string temp = current;
        while (temp.size() > b.size() || (temp.size() == b.size() && temp >= b)) {
            temp = subBigInt(temp, b);
            q++;
        }
        result += static_cast<char>('0' + q);
        current = temp;
        if (current == "0") current.clear();
    }
    while (result.size() > 1 && result[0] == '0') result.erase(0, 1);
    return result;
}

static int compareBigInt(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    return a.compare(b);
}

// ==== Alphabet Utilities ====

std::string stringToBigInt(const std::string& s, const std::string& alphabet) {
    int base = static_cast<int>(alphabet.size());
    std::string result = "0";
    std::string baseStr = std::to_string(base);
    for (char c : s) {
        int idx = static_cast<int>(alphabet.find(c));
        if (idx < 0) continue;
        result = addBigInt(mulBigInt(result, baseStr), std::to_string(idx));
    }
    return result;
}

std::string bigIntToString(const std::string& decimal, const std::string& alphabet) {
    int base = static_cast<int>(alphabet.size());
    std::string result;
    std::string current = decimal;
    std::string baseStr = std::to_string(base);
    if (current == "0") return std::string(1, alphabet[0]);
    while (current != "0") {
        std::string quotient = divBigInt(current, baseStr);
        std::string remainder = subBigInt(current, mulBigInt(quotient, baseStr));
        int idx = std::stoi(remainder);
        if (idx >= 0 && idx < base) { result += alphabet[idx]; }
        current = quotient;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ==== String Order (from StringOrderUtils.kt) ====
// Original Kotlin:
//   fun midPoints(left, right, count): List<String>

std::vector<std::string> stringMidPoints(
    const std::string& left, const std::string& right,
    int count, const std::string& alphabet)
{
    if (left == right) return {};
    if (left > right) return stringMidPoints(right, left, count, alphabet);
    size_t size = std::max(left.size(), right.size());
    std::string leftPadded = left;
    std::string rightPadded = right;
    while (leftPadded.size() < size) leftPadded += alphabet[0];
    while (rightPadded.size() < size) rightPadded += alphabet[0];
    std::string b1 = stringToBigInt(leftPadded, alphabet);
    std::string b2 = stringToBigInt(rightPadded, alphabet);
    std::string diff = subBigInt(b2, b1);
    std::string step = divBigInt(diff, std::to_string(count + 1));
    std::vector<std::string> orders;
    std::string previous = left;
    std::string current = b1;
    for (int i = 0; i < count; ++i) {
        current = addBigInt(current, step);
        std::string newOrder = bigIntToString(current, alphabet);
        if (previous >= newOrder) return {};
        orders.push_back(newOrder);
        previous = newOrder;
    }
    if (orders.empty() || orders.back() >= right) return {};
    return orders;
}

std::string stringAverage(
    const std::string& left, const std::string& right, const std::string& alphabet)
{
    auto mids = stringMidPoints(left, right, 1, alphabet);
    return mids.empty() ? "" : mids[0];
}

// ==== Space Reorder (from SpaceOrderUtils.kt:41-104) ====
// Original Kotlin:
//   fun orderCommandsForMove(orderedSpaces, movedSpaceId, delta): List<SpaceReOrderCommand>

std::vector<ReorderCommand> computeSpaceReorder(
    const std::vector<std::string>& orderedItemIds,
    const std::vector<std::string>& currentOrders,
    const std::string& movedItemId, int delta)
{
    if (delta == 0) return {};
    int movedIndex = -1;
    for (size_t i = 0; i < orderedItemIds.size(); ++i) {
        if (orderedItemIds[i] == movedItemId) { movedIndex = static_cast<int>(i); break; }
    }
    if (movedIndex < 0) return {};
    int targetIndex = (delta > 0) ? movedIndex + delta : (movedIndex + delta - 1);
    std::vector<std::string> nodesToReNumber;
    std::string lowerBoundOrder;
    int idx = targetIndex;
    while (idx >= 0 && lowerBoundOrder.empty()) {
        if (idx < static_cast<int>(orderedItemIds.size())) {
            const auto& nodeOrder = currentOrders[idx];
            if (orderedItemIds[idx] == movedItemId) break;
            if (nodeOrder.empty()) {
                nodesToReNumber.insert(nodesToReNumber.begin(), orderedItemIds[idx]);
            } else {
                lowerBoundOrder = nodeOrder;
            }
        }
        idx--;
    }
    nodesToReNumber.push_back(movedItemId);
    std::string afterOrder;
    int afterIdx = targetIndex + 1;
    if (afterIdx < static_cast<int>(orderedItemIds.size())) {
        afterOrder = currentOrders[afterIdx];
    }
    if (afterOrder.empty()) {
        afterOrder = std::string(4, DEFAULT_ORDER_ALPHABET[93]);
    }
    if (lowerBoundOrder.empty()) {
        lowerBoundOrder = std::string(4, DEFAULT_ORDER_ALPHABET[0]);
    }
    auto newOrders = stringMidPoints(lowerBoundOrder, afterOrder,
                                      static_cast<int>(nodesToReNumber.size()));
    std::vector<ReorderCommand> result;
    if (newOrders.empty()) {
        for (size_t i = 0; i < orderedItemIds.size(); ++i) {
            result.push_back({orderedItemIds[i], "order_" + std::to_string(i)});
        }
    } else {
        for (size_t i = 0; i < nodesToReNumber.size(); ++i) {
            result.push_back({nodesToReNumber[i], newOrders[i]});
        }
    }
    return result;
}

// ============================================================
// STRING NORMALIZATION — Unicode NFC/NFD/NFKC/NFKD (simplified)
// Original Kotlin:
//   java.text.Normalizer wrapper; here we implement minimal decomposition tables
// ============================================================

// Latin-1 Supplement + Latin Extended-A decomposition table (NFD)
// Maps composite character to its decomposed sequence
static const std::unordered_map<uint32_t, std::vector<uint32_t>> NFD_DECOMP = {
    // Latin-1 Supplement: À-Ö, Ø-ö, ø-ÿ
    {0x00C0, {0x0041, 0x0300}}, {0x00C1, {0x0041, 0x0301}},
    {0x00C2, {0x0041, 0x0302}}, {0x00C3, {0x0041, 0x0303}},
    {0x00C4, {0x0041, 0x0308}}, {0x00C5, {0x0041, 0x030A}},
    {0x00C7, {0x0043, 0x0327}}, {0x00C8, {0x0045, 0x0300}},
    {0x00C9, {0x0045, 0x0301}}, {0x00CA, {0x0045, 0x0302}},
    {0x00CB, {0x0045, 0x0308}}, {0x00CC, {0x0049, 0x0300}},
    {0x00CD, {0x0049, 0x0301}}, {0x00CE, {0x0049, 0x0302}},
    {0x00CF, {0x0049, 0x0308}}, {0x00D1, {0x004E, 0x0303}},
    {0x00D2, {0x004F, 0x0300}}, {0x00D3, {0x004F, 0x0301}},
    {0x00D4, {0x004F, 0x0302}}, {0x00D5, {0x004F, 0x0303}},
    {0x00D6, {0x004F, 0x0308}}, {0x00D9, {0x0055, 0x0300}},
    {0x00DA, {0x0055, 0x0301}}, {0x00DB, {0x0055, 0x0302}},
    {0x00DC, {0x0055, 0x0308}}, {0x00DD, {0x0059, 0x0301}},
    {0x00E0, {0x0061, 0x0300}}, {0x00E1, {0x0061, 0x0301}},
    {0x00E2, {0x0061, 0x0302}}, {0x00E3, {0x0061, 0x0303}},
    {0x00E4, {0x0061, 0x0308}}, {0x00E5, {0x0061, 0x030A}},
    {0x00E7, {0x0063, 0x0327}}, {0x00E8, {0x0065, 0x0300}},
    {0x00E9, {0x0065, 0x0301}}, {0x00EA, {0x0065, 0x0302}},
    {0x00EB, {0x0065, 0x0308}}, {0x00EC, {0x0069, 0x0300}},
    {0x00ED, {0x0069, 0x0301}}, {0x00EE, {0x0069, 0x0302}},
    {0x00EF, {0x0069, 0x0308}}, {0x00F1, {0x006E, 0x0303}},
    {0x00F2, {0x006F, 0x0300}}, {0x00F3, {0x006F, 0x0301}},
    {0x00F4, {0x006F, 0x0302}}, {0x00F5, {0x006F, 0x0303}},
    {0x00F6, {0x006F, 0x0308}}, {0x00F9, {0x0075, 0x0300}},
    {0x00FA, {0x0075, 0x0301}}, {0x00FB, {0x0075, 0x0302}},
    {0x00FC, {0x0075, 0x0308}}, {0x00FD, {0x0075, 0x0301}},
    {0x00FF, {0x0075, 0x0308}}, {0x0100, {0x0041, 0x0304}},
    {0x0101, {0x0061, 0x0304}}, {0x0102, {0x0041, 0x0306}},
    {0x0103, {0x0061, 0x0306}}, {0x0104, {0x0041, 0x0328}},
    {0x0105, {0x0061, 0x0328}}, {0x0106, {0x0043, 0x0301}},
    {0x0107, {0x0063, 0x0301}}, {0x0108, {0x0043, 0x0302}},
    {0x0109, {0x0063, 0x0302}}, {0x010A, {0x0043, 0x0307}},
    {0x010B, {0x0063, 0x0307}}, {0x010C, {0x0043, 0x030C}},
    {0x010D, {0x0063, 0x030C}}, {0x010E, {0x0044, 0x030C}},
    {0x010F, {0x0064, 0x030C}}, {0x0112, {0x0045, 0x0304}},
    {0x0113, {0x0065, 0x0304}}, {0x0114, {0x0045, 0x0306}},
    {0x0115, {0x0065, 0x0306}}, {0x0116, {0x0045, 0x0307}},
    {0x0117, {0x0065, 0x0307}}, {0x0118, {0x0045, 0x0328}},
    {0x0119, {0x0065, 0x0328}}, {0x011A, {0x0045, 0x030C}},
    {0x011B, {0x0065, 0x030C}}, {0x011C, {0x0047, 0x0302}},
    {0x011D, {0x0067, 0x0302}}, {0x011E, {0x0047, 0x0306}},
    {0x011F, {0x0067, 0x0306}}, {0x0120, {0x0047, 0x0307}},
    {0x0121, {0x0067, 0x0307}}, {0x0122, {0x0047, 0x0327}},
    {0x0123, {0x0067, 0x0327}}, {0x0124, {0x0048, 0x0302}},
    {0x0125, {0x0068, 0x0302}}, {0x0128, {0x0049, 0x0303}},
    {0x0129, {0x0069, 0x0303}}, {0x012A, {0x0049, 0x0304}},
    {0x012B, {0x0069, 0x0304}}, {0x012C, {0x0049, 0x0306}},
    {0x012D, {0x0069, 0x0306}}, {0x012E, {0x0049, 0x0328}},
    {0x012F, {0x0069, 0x0328}}, {0x0130, {0x0049, 0x0307}},
    {0x0134, {0x004A, 0x0302}}, {0x0135, {0x006A, 0x0302}},
    {0x0136, {0x004B, 0x0327}}, {0x0137, {0x006B, 0x0327}},
    {0x0139, {0x004C, 0x0301}}, {0x013A, {0x006C, 0x0301}},
    {0x013B, {0x004C, 0x0327}}, {0x013C, {0x006C, 0x0327}},
    {0x013D, {0x004C, 0x030C}}, {0x013E, {0x006C, 0x030C}},
    {0x0143, {0x004E, 0x0301}}, {0x0144, {0x006E, 0x0301}},
    {0x0145, {0x004E, 0x0327}}, {0x0146, {0x006E, 0x0327}},
    {0x0147, {0x004E, 0x030C}}, {0x0148, {0x006E, 0x030C}},
    {0x014C, {0x004F, 0x0304}}, {0x014D, {0x006F, 0x0304}},
    {0x014E, {0x004F, 0x0306}}, {0x014F, {0x006F, 0x0306}},
    {0x0150, {0x004F, 0x030B}}, {0x0151, {0x006F, 0x030B}},
    {0x0154, {0x0052, 0x0301}}, {0x0155, {0x0072, 0x0301}},
    {0x0156, {0x0052, 0x0327}}, {0x0157, {0x0072, 0x0327}},
    {0x0158, {0x0052, 0x030C}}, {0x0159, {0x0072, 0x030C}},
    {0x015A, {0x0053, 0x0301}}, {0x015B, {0x0073, 0x0301}},
    {0x015C, {0x0053, 0x0302}}, {0x015D, {0x0073, 0x0302}},
    {0x015E, {0x0053, 0x0327}}, {0x015F, {0x0073, 0x0327}},
    {0x0160, {0x0053, 0x030C}}, {0x0161, {0x0073, 0x030C}},
    {0x0162, {0x0054, 0x0327}}, {0x0163, {0x0074, 0x0327}},
    {0x0164, {0x0054, 0x030C}}, {0x0165, {0x0074, 0x030C}},
    {0x0168, {0x0055, 0x0303}}, {0x0169, {0x0075, 0x0303}},
    {0x016A, {0x0055, 0x0304}}, {0x016B, {0x0075, 0x0304}},
    {0x016C, {0x0055, 0x0306}}, {0x016D, {0x0075, 0x0306}},
    {0x016E, {0x0055, 0x030A}}, {0x016F, {0x0075, 0x030A}},
    {0x0170, {0x0055, 0x030B}}, {0x0171, {0x0075, 0x030B}},
    {0x0172, {0x0055, 0x0328}}, {0x0173, {0x0075, 0x0328}},
    {0x0174, {0x0057, 0x0302}}, {0x0175, {0x0077, 0x0302}},
    {0x0176, {0x0059, 0x0302}}, {0x0177, {0x0079, 0x0302}},
    {0x0178, {0x0059, 0x0308}}, {0x0179, {0x005A, 0x0301}},
    {0x017A, {0x007A, 0x0301}}, {0x017B, {0x005A, 0x0307}},
    {0x017C, {0x007A, 0x0307}}, {0x017D, {0x005A, 0x030C}},
    {0x017E, {0x007A, 0x030C}},
};

// Decompose a code point to NFD form
static std::vector<uint32_t> decomposeNFD(uint32_t cp) {
    auto it = NFD_DECOMP.find(cp);
    if (it != NFD_DECOMP.end()) return it->second;
    return {cp};
}

// Parse UTF-8 into code points
static std::vector<uint32_t> utf8ToCodePoints(const std::string& s) {
    std::vector<uint32_t> result;
    for (size_t i = 0; i < s.size(); ) {
        uint8_t b = static_cast<uint8_t>(s[i]);
        uint32_t cp;
        if (b < 0x80) { cp = b; i += 1; }
        else if ((b & 0xE0) == 0xC0) {
            if (i + 1 >= s.size()) break;
            cp = ((b & 0x1F) << 6) | (static_cast<uint8_t>(s[i+1]) & 0x3F);
            i += 2;
        } else if ((b & 0xF0) == 0xE0) {
            if (i + 2 >= s.size()) break;
            cp = ((b & 0x0F) << 12) | ((static_cast<uint8_t>(s[i+1]) & 0x3F) << 6)
               | (static_cast<uint8_t>(s[i+2]) & 0x3F);
            i += 3;
        } else if ((b & 0xF8) == 0xF0) {
            if (i + 3 >= s.size()) break;
            cp = ((b & 0x07) << 18) | ((static_cast<uint8_t>(s[i+1]) & 0x3F) << 12)
               | ((static_cast<uint8_t>(s[i+2]) & 0x3F) << 6)
               | (static_cast<uint8_t>(s[i+3]) & 0x3F);
            i += 4;
        } else {
            i += 1;
            cp = 0xFFFD;
        }
        result.push_back(cp);
    }
    return result;
}

// Encode code points back to UTF-8
static std::string codePointsToUtf8(const std::vector<uint32_t>& cps) {
    std::string result;
    for (uint32_t cp : cps) {
        if (cp < 0x80) { result += static_cast<char>(cp); }
        else if (cp < 0x800) {
            result += static_cast<char>(0xC0 | (cp >> 6));
            result += static_cast<char>(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            result += static_cast<char>(0xE0 | (cp >> 12));
            result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (cp & 0x3F));
        } else {
            result += static_cast<char>(0xF0 | (cp >> 18));
            result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
            result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (cp & 0x3F));
        }
    }
    return result;
}

// Check if code point is a combining mark (nonspacing mark)
static bool isCombiningMark(uint32_t cp) {
    return (cp >= 0x0300 && cp <= 0x036F) ||
           (cp >= 0x0483 && cp <= 0x0489) ||
           (cp >= 0x0591 && cp <= 0x05BD) ||
           (cp >= 0x05BF && cp <= 0x05C7) ||
           (cp >= 0x0610 && cp <= 0x061A) ||
           (cp >= 0x064B && cp <= 0x065F) ||
           (cp >= 0x0670 && cp <= 0x0670) ||
           (cp >= 0x06D6 && cp <= 0x06DC) ||
           (cp >= 0x06DF && cp <= 0x06E4) ||
           (cp >= 0x06E7 && cp <= 0x06E8) ||
           (cp >= 0x06EA && cp <= 0x06ED) ||
           (cp >= 0x0711 && cp <= 0x0711) ||
           (cp >= 0x0730 && cp <= 0x074A) ||
           (cp >= 0x07A6 && cp <= 0x07B0) ||
           (cp >= 0x0900 && cp <= 0x0902) ||
           (cp >= 0x093A && cp <= 0x093C) ||
           (cp >= 0x0941 && cp <= 0x0948) ||
           (cp >= 0x094D && cp <= 0x094D) ||
           (cp >= 0x0951 && cp <= 0x0957) ||
           (cp >= 0x0962 && cp <= 0x0963) ||
           (cp >= 0x0981 && cp <= 0x0983) ||
           (cp >= 0x09BC && cp <= 0x09BC) ||
           (cp >= 0x09C1 && cp <= 0x09C4) ||
           (cp >= 0x09CD && cp <= 0x09CD) ||
           (cp >= 0x09E2 && cp <= 0x09E3) ||
           (cp >= 0x0A01 && cp <= 0x0A03) ||
           (cp >= 0x0A3C && cp <= 0x0A3C) ||
           (cp >= 0x0A41 && cp <= 0x0A42) ||
           (cp >= 0x0A47 && cp <= 0x0A48) ||
           (cp >= 0x0A4B && cp <= 0x0A4D) ||
           (cp >= 0x0A70 && cp <= 0x0A71) ||
           (cp >= 0x0A81 && cp <= 0x0A83) ||
           (cp >= 0x0ABC && cp <= 0x0ABC) ||
           (cp >= 0x0AC1 && cp <= 0x0AC5) ||
           (cp >= 0x0AC7 && cp <= 0x0AC9) ||
           (cp >= 0x0ACD && cp <= 0x0ACD) ||
           (cp >= 0x0AE2 && cp <= 0x0AE3) ||
           (cp >= 0x0B01 && cp <= 0x0B03) ||
           (cp >= 0x0B3C && cp <= 0x0B3C) ||
           (cp >= 0x0B3F && cp <= 0x0B3F) ||
           (cp >= 0x0B41 && cp <= 0x0B44) ||
           (cp >= 0x0B4D && cp <= 0x0B4D) ||
           (cp >= 0x0B56 && cp <= 0x0B56) ||
           (cp >= 0x0B82 && cp <= 0x0B82) ||
           (cp >= 0x0BC0 && cp <= 0x0BC0) ||
           (cp >= 0x0BCD && cp <= 0x0BCD) ||
           (cp >= 0x0C3E && cp <= 0x0C40) ||
           (cp >= 0x0C46 && cp <= 0x0C48) ||
           (cp >= 0x0C4A && cp <= 0x0C4D) ||
           (cp >= 0x0C55 && cp <= 0x0C56) ||
           (cp >= 0x0CBC && cp <= 0x0CBC) ||
           (cp >= 0x0CBF && cp <= 0x0CBF) ||
           (cp >= 0x0CC6 && cp <= 0x0CC6) ||
           (cp >= 0x0CCC && cp <= 0x0CCD) ||
           (cp >= 0x0CE2 && cp <= 0x0CE3) ||
           (cp >= 0x0D41 && cp <= 0x0D44) ||
           (cp >= 0x0D4D && cp <= 0x0D4D) ||
           (cp >= 0x0DCA && cp <= 0x0DCA) ||
           (cp >= 0x0DD2 && cp <= 0x0DD4) ||
           (cp >= 0x0DD6 && cp <= 0x0DD6) ||
           (cp >= 0x0E31 && cp <= 0x0E31) ||
           (cp >= 0x0E34 && cp <= 0x0E3A) ||
           (cp >= 0x0E47 && cp <= 0x0E4E) ||
           (cp >= 0x0EB1 && cp <= 0x0EB1) ||
           (cp >= 0x0EB4 && cp <= 0x0EB9) ||
           (cp >= 0x0EBB && cp <= 0x0EBC) ||
           (cp >= 0x0EC8 && cp <= 0x0ECD) ||
           (cp >= 0x0F18 && cp <= 0x0F19) ||
           (cp >= 0x0F35 && cp <= 0x0F35) ||
           (cp >= 0x0F37 && cp <= 0x0F37) ||
           (cp >= 0x0F39 && cp <= 0x0F39) ||
           (cp >= 0x0F71 && cp <= 0x0F7E) ||
           (cp >= 0x0F80 && cp <= 0x0F84) ||
           (cp >= 0x0F86 && cp <= 0x0F87) ||
           (cp >= 0x0F90 && cp <= 0x0F97) ||
           (cp >= 0x0F99 && cp <= 0x0FBC) ||
           (cp >= 0x0FC6 && cp <= 0x0FC6) ||
           (cp >= 0x1037 && cp <= 0x1037) ||
           (cp >= 0x1039 && cp <= 0x103A) ||
           (cp >= 0x108D && cp <= 0x108D) ||
           (cp >= 0x135D && cp <= 0x135F) ||
           (cp >= 0x1714 && cp <= 0x1714) ||
           (cp >= 0x1734 && cp <= 0x1734) ||
           (cp >= 0x17D2 && cp <= 0x17D2) ||
           (cp >= 0x17DD && cp <= 0x17DD) ||
           (cp >= 0x18A9 && cp <= 0x18A9) ||
           (cp >= 0x1920 && cp <= 0x1922) ||
           (cp >= 0x1927 && cp <= 0x1928) ||
           (cp >= 0x1932 && cp <= 0x1932) ||
           (cp >= 0x1939 && cp <= 0x193B) ||
           (cp >= 0x1A17 && cp <= 0x1A18) ||
           (cp >= 0x1B00 && cp <= 0x1B03) ||
           (cp >= 0x1B34 && cp <= 0x1B34) ||
           (cp >= 0x1B36 && cp <= 0x1B3A) ||
           (cp >= 0x1B3C && cp <= 0x1B3C) ||
           (cp >= 0x1B42 && cp <= 0x1B42) ||
           (cp >= 0x1B6B && cp <= 0x1B73) ||
           (cp >= 0x1DC0 && cp <= 0x1DFF) ||
           (cp >= 0x20D0 && cp <= 0x20FF) ||
           (cp >= 0x2CEF && cp <= 0x2CF1) ||
           (cp >= 0x2D7F && cp <= 0x2D7F) ||
           (cp >= 0x2DE0 && cp <= 0x2DFF) ||
           (cp >= 0xA66F && cp <= 0xA672) ||
           (cp >= 0xA674 && cp <= 0xA67D) ||
           (cp >= 0xA69E && cp <= 0xA69F) ||
           (cp >= 0xA6F0 && cp <= 0xA6F1) ||
           (cp >= 0xA802 && cp <= 0xA802) ||
           (cp >= 0xA806 && cp <= 0xA806) ||
           (cp >= 0xA80B && cp <= 0xA80B) ||
           (cp >= 0xA825 && cp <= 0xA826) ||
           (cp >= 0xA8C4 && cp <= 0xA8C5) ||
           (cp >= 0xA8E0 && cp <= 0xA8F1) ||
           (cp >= 0xA926 && cp <= 0xA92D) ||
           (cp >= 0xA947 && cp <= 0xA951) ||
           (cp >= 0xA980 && cp <= 0xA982) ||
           (cp >= 0xA9B3 && cp <= 0xA9B3) ||
           (cp >= 0xA9B6 && cp <= 0xA9B9) ||
           (cp >= 0xA9BC && cp <= 0xA9BC) ||
           (cp >= 0xAA29 && cp <= 0xAA2E) ||
           (cp >= 0xAA31 && cp <= 0xAA32) ||
           (cp >= 0xAA35 && cp <= 0xAA36) ||
           (cp >= 0xAA43 && cp <= 0xAA43) ||
           (cp >= 0xAA4C && cp <= 0xAA4C) ||
           (cp >= 0xAAB0 && cp <= 0xAAB0) ||
           (cp >= 0xAAB2 && cp <= 0xAAB4) ||
           (cp >= 0xAAB7 && cp <= 0xAAB8) ||
           (cp >= 0xAABE && cp <= 0xAABF) ||
           (cp >= 0xAAC1 && cp <= 0xAAC1) ||
           (cp >= 0xABE5 && cp <= 0xABE5) ||
           (cp >= 0xABE8 && cp <= 0xABE8) ||
           (cp >= 0xABED && cp <= 0xABED) ||
           (cp >= 0xFB1E && cp <= 0xFB1E) ||
           (cp >= 0xFE00 && cp <= 0xFE0F) ||
           (cp >= 0xFE20 && cp <= 0xFE2F);
}

// Compatibility decompositions (NFKC/NFKD) — common ligatures and symbols
static const std::unordered_map<uint32_t, std::vector<uint32_t>> NFKD_COMPAT = {
    {0x00A0, {0x0020}},       // no-break space → space
    {0x00B5, {0x03BC}},       // µ → μ
    {0x0132, {0x0049, 0x004A}}, {0x0133, {0x0069, 0x006A}},
    {0x013F, {0x004C, 0x00B7}}, {0x0140, {0x006C, 0x00B7}},
    {0x0149, {0x02BC, 0x006E}},
    {0x017F, {0x0073}},       // ſ → s
    {0x01C4, {0x0044, 0x005A, 0x030C}}, {0x01C5, {0x0044, 0x007A, 0x030C}},
    {0x01C6, {0x0064, 0x007A, 0x030C}}, {0x01C7, {0x004C, 0x004A}},
    {0x01C8, {0x004C, 0x006A}}, {0x01C9, {0x006C, 0x006A}},
    {0x01CA, {0x004E, 0x004A}}, {0x01CB, {0x004E, 0x006A}},
    {0x01CC, {0x006E, 0x006A}},
    {0x01F1, {0x0044, 0x005A}}, {0x01F2, {0x0044, 0x007A}},
    {0x01F3, {0x0064, 0x007A}},
    {0xFB00, {0x0066, 0x0066}}, {0xFB01, {0x0066, 0x0069}},
    {0xFB02, {0x0066, 0x006C}}, {0xFB03, {0x0066, 0x0066, 0x0069}},
    {0xFB04, {0x0066, 0x0066, 0x006C}}, {0xFB05, {0x0073, 0x0074}},
    {0xFB06, {0x0073, 0x0074}},
    {0x2126, {0x03A9}},       // Ω → Ω (Greek)
    {0x212A, {0x004B}},       // K → K
    {0x212B, {0x00C5}},       // Å → Å
    {0x2160, {0x0049}}, {0x2161, {0x0049, 0x0049}}, {0x2162, {0x0049, 0x0049, 0x0049}},
    {0x2163, {0x0049, 0x0056}}, {0x2164, {0x0056}}, {0x2165, {0x0056, 0x0049}},
    {0x2166, {0x0056, 0x0049, 0x0049}}, {0x2167, {0x0056, 0x0049, 0x0049, 0x0049}},
    {0x2168, {0x0049, 0x0058}}, {0x2169, {0x0058}}, {0x216A, {0x0058, 0x0049}},
    {0x216B, {0x0058, 0x0049, 0x0049}}, {0x216C, {0x004C}},
    {0x216D, {0x0043}}, {0x216E, {0x0044}}, {0x216F, {0x004D}},
    {0x2170, {0x0069}}, {0x2171, {0x0069, 0x0069}}, {0x2172, {0x0069, 0x0069, 0x0069}},
    {0x2173, {0x0069, 0x0076}}, {0x2174, {0x0076}}, {0x2175, {0x0076, 0x0069}},
    {0x2176, {0x0076, 0x0069, 0x0069}}, {0x2177, {0x0076, 0x0069, 0x0069, 0x0069}},
    {0x2178, {0x0069, 0x0078}}, {0x2179, {0x0078}}, {0x217A, {0x0078, 0x0069}},
    {0x217B, {0x0078, 0x0069, 0x0069}}, {0x217C, {0x006C}},
    {0x217D, {0x0063}}, {0x217E, {0x0064}}, {0x217F, {0x006D}},
};

static std::vector<uint32_t> decomposeChar(uint32_t cp, bool compatibility) {
    if (compatibility) {
        auto cit = NFKD_COMPAT.find(cp);
        if (cit != NFKD_COMPAT.end()) return cit->second;
    }
    return decomposeNFD(cp);
}

// Canonical ordering: sort combining marks by canonical combining class
static int combiningClass(uint32_t cp) {
    if (cp < 0x300 || cp > 0x36F) return 0;
    static const std::unordered_map<uint32_t, int> cc = {
        {0x0300, 230}, {0x0301, 230}, {0x0302, 230}, {0x0303, 230},
        {0x0304, 230}, {0x0305, 230}, {0x0306, 230}, {0x0307, 230},
        {0x0308, 230}, {0x0309, 230}, {0x030A, 230}, {0x030B, 230},
        {0x030C, 230}, {0x030D, 230}, {0x030E, 230}, {0x030F, 230},
        {0x0310, 230}, {0x0311, 230}, {0x0312, 230}, {0x0313, 230},
        {0x0314, 230}, {0x0315, 232}, {0x0316, 220}, {0x0317, 220},
        {0x0318, 220}, {0x0319, 220}, {0x031A, 232}, {0x031B, 216},
        {0x031C, 220}, {0x031D, 220}, {0x031E, 220}, {0x031F, 220},
        {0x0320, 220}, {0x0321, 202}, {0x0322, 202}, {0x0323, 220},
        {0x0324, 220}, {0x0325, 220}, {0x0326, 220}, {0x0327, 202},
        {0x0328, 202}, {0x0329, 220}, {0x032A, 220}, {0x032B, 220},
        {0x032C, 220}, {0x032D, 220}, {0x032E, 220}, {0x032F, 220},
        {0x0330, 220}, {0x0331, 220}, {0x0332, 220}, {0x0333, 220},
        {0x0334, 1},   {0x0335, 1},   {0x0336, 1},   {0x0337, 1},
        {0x0338, 1},   {0x0339, 220}, {0x033A, 220}, {0x033B, 220},
        {0x033C, 220}, {0x033D, 230}, {0x033E, 230}, {0x033F, 230},
        {0x0340, 230}, {0x0341, 230}, {0x0342, 230}, {0x0343, 230},
        {0x0344, 230}, {0x0345, 240}, {0x0346, 230}, {0x0347, 220},
        {0x0348, 220}, {0x0349, 220}, {0x034A, 230}, {0x034B, 230},
        {0x034C, 230}, {0x034D, 220}, {0x034E, 220}, {0x0350, 230},
        {0x0351, 230}, {0x0352, 230}, {0x0357, 230},
    };
    auto it = cc.find(cp);
    return it != cc.end() ? it->second : 0;
}

static std::vector<uint32_t> canonicalOrdering(const std::vector<uint32_t>& cps) {
    std::vector<uint32_t> result;
    for (size_t i = 0; i < cps.size(); ) {
        uint32_t base = cps[i];
        result.push_back(base);
        i++;
        if (isCombiningMark(base)) continue;
        // Collect following combining marks
        std::vector<std::pair<int, uint32_t>> marks;
        while (i < cps.size() && isCombiningMark(cps[i])) {
            marks.push_back({combiningClass(cps[i]), cps[i]});
            i++;
        }
        // Sort by combining class (stable sort)
        std::stable_sort(marks.begin(), marks.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& m : marks) result.push_back(m.second);
    }
    return result;
}

// NFC composition: map base + combining mark back to composite
static bool composeNFC(uint32_t base, uint32_t mark, uint32_t& out) {
    static const std::map<std::pair<uint32_t, uint32_t>, uint32_t> compose = {
        {{0x0041, 0x0300}, 0x00C0}, {{0x0041, 0x0301}, 0x00C1},
        {{0x0041, 0x0302}, 0x00C2}, {{0x0041, 0x0303}, 0x00C3},
        {{0x0041, 0x0308}, 0x00C4}, {{0x0041, 0x030A}, 0x00C5},
        {{0x0043, 0x0327}, 0x00C7}, {{0x0045, 0x0300}, 0x00C8},
        {{0x0045, 0x0301}, 0x00C9}, {{0x0045, 0x0302}, 0x00CA},
        {{0x0045, 0x0308}, 0x00CB}, {{0x0049, 0x0300}, 0x00CC},
        {{0x0049, 0x0301}, 0x00CD}, {{0x0049, 0x0302}, 0x00CE},
        {{0x0049, 0x0308}, 0x00CF}, {{0x004E, 0x0303}, 0x00D1},
        {{0x004F, 0x0300}, 0x00D2}, {{0x004F, 0x0301}, 0x00D3},
        {{0x004F, 0x0302}, 0x00D4}, {{0x004F, 0x0303}, 0x00D5},
        {{0x004F, 0x0308}, 0x00D6}, {{0x0055, 0x0300}, 0x00D9},
        {{0x0055, 0x0301}, 0x00DA}, {{0x0055, 0x0302}, 0x00DB},
        {{0x0055, 0x0308}, 0x00DC}, {{0x0059, 0x0301}, 0x00DD},
        {{0x0061, 0x0300}, 0x00E0}, {{0x0061, 0x0301}, 0x00E1},
        {{0x0061, 0x0302}, 0x00E2}, {{0x0061, 0x0303}, 0x00E3},
        {{0x0061, 0x0308}, 0x00E4}, {{0x0061, 0x030A}, 0x00E5},
        {{0x0063, 0x0327}, 0x00E7}, {{0x0065, 0x0300}, 0x00E8},
        {{0x0065, 0x0301}, 0x00E9}, {{0x0065, 0x0302}, 0x00EA},
        {{0x0065, 0x0308}, 0x00EB}, {{0x0069, 0x0300}, 0x00EC},
        {{0x0069, 0x0301}, 0x00ED}, {{0x0069, 0x0302}, 0x00EE},
        {{0x0069, 0x0308}, 0x00EF}, {{0x006E, 0x0303}, 0x00F1},
        {{0x006F, 0x0300}, 0x00F2}, {{0x006F, 0x0301}, 0x00F3},
        {{0x006F, 0x0302}, 0x00F4}, {{0x006F, 0x0303}, 0x00F5},
        {{0x006F, 0x0308}, 0x00F6}, {{0x0075, 0x0300}, 0x00F9},
        {{0x0075, 0x0301}, 0x00FA}, {{0x0075, 0x0302}, 0x00FB},
        {{0x0075, 0x0308}, 0x00FC},
    };
    auto it = compose.find({base, mark});
    if (it != compose.end()) { out = it->second; return true; }
    return false;
}

static std::vector<uint32_t> composeNFC(const std::vector<uint32_t>& decomposed) {
    if (decomposed.empty()) return decomposed;
    std::vector<uint32_t> result;
    for (size_t i = 0; i < decomposed.size(); ) {
        uint32_t c = decomposed[i++];
        // Try to compose with next combining marks
        while (i < decomposed.size() && isCombiningMark(decomposed[i])) {
            uint32_t composite;
            if (composeNFC(c, decomposed[i], composite)) {
                c = composite;
                i++;
            } else {
                break;
            }
        }
        result.push_back(c);
    }
    return result;
}

std::string normalizeString(const std::string& input, StringNormalization form) {
    // Original Kotlin:
    //   java.text.Normalizer.normalize(input, Normalizer.Form.NFC/NFD/NFKC/NFKD)
    auto cps = utf8ToCodePoints(input);
    bool compatibility = (form == StringNormalization::NFKC || form == StringNormalization::NFKD);

    // Step 1: Decompose
    std::vector<uint32_t> decomposed;
    for (uint32_t cp : cps) {
        auto parts = decomposeChar(cp, compatibility);
        if (compatibility) {
            // Recursively decompose compatibility results
            for (uint32_t p : parts) {
                auto sub = decomposeChar(p, false);
                decomposed.insert(decomposed.end(), sub.begin(), sub.end());
            }
        } else {
            decomposed.insert(decomposed.end(), parts.begin(), parts.end());
        }
    }

    // Step 2: Canonical ordering
    auto ordered = canonicalOrdering(decomposed);

    // Step 3: Compose for NFC / NFKC
    if (form == StringNormalization::NFC || form == StringNormalization::NFKC) {
        ordered = composeNFC(ordered);
    }

    return codePointsToUtf8(ordered);
}

// ============================================================
// STRING COLLATION — locale-aware string comparison
// Original Kotlin:
//   java.text.Collator.getInstance(locale)
//   Implements Unicode Collation Algorithm (UCA) simplified
// ============================================================

static uint32_t collationPrimaryWeight(uint32_t cp) {
    // Map letters to primary sort weight (ignoring case and accents)
    if (cp >= 'A' && cp <= 'Z') return cp + 32;
    if (cp >= 'a' && cp <= 'z') return cp;
    // Map accented latin characters to their base
    static const std::unordered_map<uint32_t, uint32_t> baseMap = {
        {0x00E0, 'a'}, {0x00E1, 'a'}, {0x00E2, 'a'}, {0x00E3, 'a'},
        {0x00E4, 'a'}, {0x00E5, 'a'}, {0x00E8, 'e'}, {0x00E9, 'e'},
        {0x00EA, 'e'}, {0x00EB, 'e'}, {0x00EC, 'i'}, {0x00ED, 'i'},
        {0x00EE, 'i'}, {0x00EF, 'i'}, {0x00F1, 'n'}, {0x00F2, 'o'},
        {0x00F3, 'o'}, {0x00F4, 'o'}, {0x00F5, 'o'}, {0x00F6, 'o'},
        {0x00F9, 'u'}, {0x00FA, 'u'}, {0x00FB, 'u'}, {0x00FC, 'u'},
        {0x00C0, 'A' + 32}, {0x00C1, 'A' + 32}, {0x00C2, 'A' + 32},
        {0x00C3, 'A' + 32}, {0x00C4, 'A' + 32}, {0x00C5, 'A' + 32},
        {0x00C8, 'E' + 32}, {0x00C9, 'E' + 32}, {0x00CA, 'E' + 32},
        {0x00CB, 'E' + 32}, {0x00CC, 'I' + 32}, {0x00CD, 'I' + 32},
        {0x00CE, 'I' + 32}, {0x00CF, 'I' + 32}, {0x00D1, 'N' + 32},
        {0x00D2, 'O' + 32}, {0x00D3, 'O' + 32}, {0x00D4, 'O' + 32},
        {0x00D5, 'O' + 32}, {0x00D6, 'O' + 32}, {0x00D9, 'U' + 32},
        {0x00DA, 'U' + 32}, {0x00DB, 'U' + 32}, {0x00DC, 'U' + 32},
    };
    auto it = baseMap.find(cp);
    return it != baseMap.end() ? it->second : cp;
}

static uint32_t collationTertiaryCase(uint32_t cp) {
    // 0 = lowercase, 1 = uppercase
    if (cp >= 'A' && cp <= 'Z') return 1;
    static const std::unordered_set<uint32_t> uppercaseAccented = {
        0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5,
        0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD,
        0x00CE, 0x00CF, 0x00D1, 0x00D2, 0x00D3, 0x00D4,
        0x00D5, 0x00D6, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD,
    };
    return uppercaseAccented.count(cp) ? 1 : 0;
}

static int32_t collateCodePoints(const std::vector<uint32_t>& aCps,
                                  const std::vector<uint32_t>& bCps,
                                  StringCollation strength) {
    size_t maxLen = std::max(aCps.size(), bCps.size());
    for (size_t i = 0; i < maxLen; ++i) {
        uint32_t ac = i < aCps.size() ? aCps[i] : 0;
        uint32_t bc = i < bCps.size() ? bCps[i] : 0;
        if (ac == 0 && bc == 0) return 0;
        if (ac == 0) return -1;
        if (bc == 0) return 1;

        if (strength >= StringCollation::PRIMARY) {
            uint32_t apw = collationPrimaryWeight(ac);
            uint32_t bpw = collationPrimaryWeight(bc);
            if (apw < bpw) return -1;
            if (apw > bpw) return 1;
        }
        if (strength >= StringCollation::TERTIARY) {
            if (ac < bc) return -1;
            if (ac > bc) return 1;
        } else if (strength >= StringCollation::SECONDARY) {
            // Secondary ignores case but considers accents
            if (ac != bc) {
                uint32_t aBase = collationPrimaryWeight(ac);
                uint32_t bBase = collationPrimaryWeight(bc);
                if (aBase < bBase) return -1;
                if (aBase > bBase) return 1;
            }
        }
    }
    return 0;
}

int collateStrings(const std::string& a, const std::string& b,
                    StringCollation strength, const std::string& locale) {
    // Original Kotlin:
    //   val collator = Collator.getInstance(Locale(locale))
    //   collator.strength = when(strength) { PRIMARY -> Collator.PRIMARY ... }
    //   collator.compare(a, b)
    (void)locale; // locale hint for future expansion, current impl is UCA-based
    auto aCps = utf8ToCodePoints(normalizeString(a, StringNormalization::NFD));
    auto bCps = utf8ToCodePoints(normalizeString(b, StringNormalization::NFD));
    return collateCodePoints(aCps, bCps, strength);
}

int compareStrings(const std::string& a, const std::string& b) {
    return collateStrings(a, b, StringCollation::TERTIARY);
}

// ==== StringSorter ====

bool StringSorter::operator()(const std::string& a, const std::string& b) const {
    int cmp = 0;
    switch (strategy) {
        case SortStrategy::LOCALE_DEFAULT:
            cmp = collateStrings(a, b, strength, "en_US");
            break;
        case SortStrategy::LOCALE_CASE_INSENSITIVE:
            cmp = collateStrings(a, b, StringCollation::SECONDARY, locale);
            break;
        case SortStrategy::NATURAL:
            cmp = naturalCompare(a, b);
            break;
        case SortStrategy::ASCII_BET:
            cmp = a.compare(b);
            break;
        case SortStrategy::LENGTH_ASCENDING:
            cmp = (a.size() < b.size()) ? -1 : ((a.size() > b.size()) ? 1 : a.compare(b));
            break;
        case SortStrategy::LENGTH_DESCENDING:
            cmp = (a.size() > b.size()) ? -1 : ((a.size() < b.size()) ? 1 : a.compare(b));
            break;
        case SortStrategy::LOCALE_SPECIFIC:
            cmp = collateStrings(a, b, strength, locale);
            break;
    }
    return ascending ? (cmp < 0) : (cmp > 0);
}

std::vector<std::string> sortStringsByLocale(
    const std::vector<std::string>& strings,
    const std::string& locale, StringCollation strength)
{
    auto result = strings;
    StringSorter sorter;
    sorter.strategy = SortStrategy::LOCALE_SPECIFIC;
    sorter.locale = locale;
    sorter.strength = strength;
    std::sort(result.begin(), result.end(), sorter);
    return result;
}

std::vector<std::string> getAvailableLocales() {
    // Original Kotlin:
    //   Locale.getAvailableLocales().map { it.toLanguageTag() }
    // Static list of commonly supported locales on Android
    return {
        "en_US", "en_GB", "en_AU", "en_CA", "en_IN",
        "es_ES", "es_MX", "es_AR",
        "fr_FR", "fr_CA",
        "de_DE", "de_AT", "de_CH",
        "it_IT", "pt_BR", "pt_PT",
        "ru_RU", "ja_JP", "ko_KR",
        "zh_CN", "zh_TW", "zh_HK",
        "ar_SA", "ar_EG",
        "hi_IN", "bn_IN", "ur_PK",
        "nl_NL", "pl_PL", "tr_TR",
        "sv_SE", "da_DK", "fi_FI",
        "nb_NO", "cs_CZ", "sk_SK",
        "hu_HU", "ro_RO", "uk_UA",
        "vi_VN", "th_TH", "id_ID",
        "ms_MY", "fil_PH", "he_IL",
    };
}

std::string getDefaultLocale() {
    // Original Kotlin:
    //   Locale.getDefault().toLanguageTag()
    // On Android NDK, fall back to en_US as default
    return "en_US";
}

// ============================================================
// NATURAL SORT — "file10" after "file2"
// Original Kotlin:
//   Natural-order comparator splitting on digit boundaries
// ============================================================

std::vector<StringToken> tokenizeForNaturalSort(const std::string& s) {
    std::vector<StringToken> tokens;
    if (s.empty()) return tokens;
    size_t i = 0;
    while (i < s.size()) {
        if (static_cast<unsigned char>(s[i]) >= '0' &&
            static_cast<unsigned char>(s[i]) <= '9') {
            StringToken tok;
            tok.isNumber = true;
            while (i < s.size() && static_cast<unsigned char>(s[i]) >= '0' &&
                   static_cast<unsigned char>(s[i]) <= '9') {
                tok.textPart += s[i];
                i++;
            }
            try { tok.numberPart = std::stoll(tok.textPart); }
            catch (...) { tok.numberPart = 0; }
            tokens.push_back(tok);
        } else {
            StringToken tok;
            tok.isNumber = false;
            while (i < s.size() && !(static_cast<unsigned char>(s[i]) >= '0' &&
                                       static_cast<unsigned char>(s[i]) <= '9')) {
                tok.textPart += s[i];
                i++;
            }
            tokens.push_back(tok);
        }
    }
    return tokens;
}

int naturalCompare(const std::string& a, const std::string& b) {
    // Original Kotlin:
    //   Comparator that splits on digit boundaries and compares numbers numerically
    auto aToks = tokenizeForNaturalSort(a);
    auto bToks = tokenizeForNaturalSort(b);
    size_t maxLen = std::max(aToks.size(), bToks.size());
    for (size_t i = 0; i < maxLen; ++i) {
        if (i >= aToks.size()) return -1;
        if (i >= bToks.size()) return 1;
        const auto& at = aToks[i];
        const auto& bt = bToks[i];
        if (at.isNumber && bt.isNumber) {
            if (at.numberPart < bt.numberPart) return -1;
            if (at.numberPart > bt.numberPart) return 1;
        } else if (at.isNumber && !bt.isNumber) {
            return -1;
        } else if (!at.isNumber && bt.isNumber) {
            return 1;
        } else {
            int cmp = at.textPart.compare(bt.textPart);
            if (cmp != 0) return cmp;
        }
    }
    return 0;
}

// ============================================================
// UNICODE UTILITIES
// ============================================================

UnicodeCategory getUnicodeCategory(uint32_t codePoint) {
    // ASCII
    if (codePoint < 0x80) {
        if (codePoint >= 'A' && codePoint <= 'Z') return UnicodeCategory::LETTER_UPPERCASE;
        if (codePoint >= 'a' && codePoint <= 'z') return UnicodeCategory::LETTER_LOWERCASE;
        if (codePoint >= '0' && codePoint <= '9') return UnicodeCategory::NUMBER_DECIMAL;
        if (codePoint == ' ' || codePoint == '\t' || codePoint == '\n' ||
            codePoint == '\r' || codePoint == '\f' || codePoint == '\v')
            return UnicodeCategory::SEPARATOR_SPACE;
        if ((codePoint >= 0x21 && codePoint <= 0x2F) ||
            (codePoint >= 0x3A && codePoint <= 0x40) ||
            (codePoint >= 0x5B && codePoint <= 0x60) ||
            (codePoint >= 0x7B && codePoint <= 0x7E))
            return UnicodeCategory::PUNCTUATION_OTHER;
        if (codePoint < 0x20) return UnicodeCategory::OTHER_CONTROL;
        return UnicodeCategory::UNKNOWN;
    }
    // Latin-1 Supplement (U+0080 - U+00FF)
    if (codePoint >= 0x00C0 && codePoint <= 0x00D6) return UnicodeCategory::LETTER_UPPERCASE;
    if (codePoint >= 0x00D8 && codePoint <= 0x00DE) return UnicodeCategory::LETTER_UPPERCASE;
    if (codePoint >= 0x00E0 && codePoint <= 0x00F6) return UnicodeCategory::LETTER_LOWERCASE;
    if (codePoint >= 0x00F8 && codePoint <= 0x00FE) return UnicodeCategory::LETTER_LOWERCASE;
    if (codePoint == 0x00A0) return UnicodeCategory::SEPARATOR_SPACE;
    if (codePoint >= 0x0080 && codePoint <= 0x009F) return UnicodeCategory::OTHER_CONTROL;
    // Latin Extended-A
    if (codePoint >= 0x0100 && codePoint <= 0x017F) {
        if (codePoint >= 0x0100 && codePoint <= 0x012F) {
            return (codePoint % 2 == 0) ? UnicodeCategory::LETTER_UPPERCASE
                                         : UnicodeCategory::LETTER_LOWERCASE;
        }
        if (codePoint >= 0x0130 && codePoint <= 0x0137) {
            if (codePoint == 0x0131) return UnicodeCategory::LETTER_LOWERCASE;
            if (codePoint >= 0x0132 && codePoint <= 0x0133)
                return (codePoint % 2 == 0) ? UnicodeCategory::LETTER_UPPERCASE
                                            : UnicodeCategory::LETTER_LOWERCASE;
            return (codePoint % 2 == 0) ? UnicodeCategory::LETTER_UPPERCASE
                                         : UnicodeCategory::LETTER_LOWERCASE;
        }
        if (codePoint >= 0x0139 && codePoint <= 0x0148)
            return (codePoint % 2 != 0) ? UnicodeCategory::LETTER_UPPERCASE
                                         : UnicodeCategory::LETTER_LOWERCASE;
        if (codePoint >= 0x014A && codePoint <= 0x0177)
            return (codePoint % 2 == 0) ? UnicodeCategory::LETTER_UPPERCASE
                                         : UnicodeCategory::LETTER_LOWERCASE;
        if (codePoint >= 0x0179 && codePoint <= 0x017E)
            return (codePoint % 2 != 0) ? UnicodeCategory::LETTER_UPPERCASE
                                         : UnicodeCategory::LETTER_LOWERCASE;
        return UnicodeCategory::LETTER_OTHER;
    }
    // General Unicode categories (simplified ranges)
    if (codePoint >= 0x0180 && codePoint <= 0x024F) return UnicodeCategory::LETTER_OTHER;
    if (codePoint >= 0x2000 && codePoint <= 0x200A) return UnicodeCategory::SEPARATOR_SPACE;
    if (codePoint == 0x200B) return UnicodeCategory::OTHER_FORMAT; // ZWSP
    if (codePoint >= 0x200C && codePoint <= 0x200F) return UnicodeCategory::OTHER_FORMAT;
    if (codePoint >= 0x2028 && codePoint <= 0x2029) return UnicodeCategory::SEPARATOR_PARAGRAPH;
    if (codePoint == 0x202F) return UnicodeCategory::SEPARATOR_SPACE;
    if (codePoint >= 0x205F && codePoint <= 0x2064) return UnicodeCategory::OTHER_FORMAT;
    // CJK ranges (very simplified)
    if (codePoint >= 0x4E00 && codePoint <= 0x9FFF) return UnicodeCategory::LETTER_OTHER;
    if (codePoint >= 0x3400 && codePoint <= 0x4DBF) return UnicodeCategory::LETTER_OTHER;
    // Hangul
    if (codePoint >= 0xAC00 && codePoint <= 0xD7AF) return UnicodeCategory::LETTER_OTHER;
    // Surrogates
    if (codePoint >= 0xD800 && codePoint <= 0xDFFF) return UnicodeCategory::OTHER_SURROGATE;
    // Private use
    if (codePoint >= 0xE000 && codePoint <= 0xF8FF) return UnicodeCategory::OTHER_PRIVATE;

    return UnicodeCategory::OTHER_NOT_ASSIGNED;
}

bool isLetter(uint32_t codePoint) {
    auto cat = getUnicodeCategory(codePoint);
    return cat == UnicodeCategory::LETTER_UPPERCASE ||
           cat == UnicodeCategory::LETTER_LOWERCASE ||
           cat == UnicodeCategory::LETTER_TITLECASE ||
           cat == UnicodeCategory::LETTER_MODIFIER ||
           cat == UnicodeCategory::LETTER_OTHER;
}

bool isDigit(uint32_t codePoint) {
    auto cat = getUnicodeCategory(codePoint);
    return cat == UnicodeCategory::NUMBER_DECIMAL ||
           cat == UnicodeCategory::NUMBER_LETTER;
}

bool isWhitespace(uint32_t codePoint) {
    auto cat = getUnicodeCategory(codePoint);
    return cat == UnicodeCategory::SEPARATOR_SPACE ||
           cat == UnicodeCategory::SEPARATOR_LINE ||
           cat == UnicodeCategory::SEPARATOR_PARAGRAPH ||
           codePoint == '\t' || codePoint == '\n' || codePoint == '\r' ||
           codePoint == '\f' || codePoint == '\v';
}

bool isPunctuation(uint32_t codePoint) {
    auto cat = getUnicodeCategory(codePoint);
    return cat == UnicodeCategory::PUNCTUATION_CONNECTOR ||
           cat == UnicodeCategory::PUNCTUATION_DASH ||
           cat == UnicodeCategory::PUNCTUATION_OPEN ||
           cat == UnicodeCategory::PUNCTUATION_CLOSE ||
           cat == UnicodeCategory::PUNCTUATION_INITIAL ||
           cat == UnicodeCategory::PUNCTUATION_FINAL ||
           cat == UnicodeCategory::PUNCTUATION_OTHER;
}

bool isUpperCase(uint32_t codePoint) {
    return getUnicodeCategory(codePoint) == UnicodeCategory::LETTER_UPPERCASE;
}

bool isLowerCase(uint32_t codePoint) {
    return getUnicodeCategory(codePoint) == UnicodeCategory::LETTER_LOWERCASE;
}

uint32_t toUpperCase(uint32_t codePoint) {
    // ASCII
    if (codePoint >= 'a' && codePoint <= 'z') return codePoint - 32;
    // Latin-1 Supplement lowercase to uppercase
    static const std::unordered_map<uint32_t, uint32_t> lowerToUpper = {
        {0x00E0, 0x00C0}, {0x00E1, 0x00C1}, {0x00E2, 0x00C2},
        {0x00E3, 0x00C3}, {0x00E4, 0x00C4}, {0x00E5, 0x00C5},
        {0x00E7, 0x00C7}, {0x00E8, 0x00C8}, {0x00E9, 0x00C9},
        {0x00EA, 0x00CA}, {0x00EB, 0x00CB}, {0x00EC, 0x00CC},
        {0x00ED, 0x00CD}, {0x00EE, 0x00CE}, {0x00EF, 0x00CF},
        {0x00F1, 0x00D1}, {0x00F2, 0x00D2}, {0x00F3, 0x00D3},
        {0x00F4, 0x00D4}, {0x00F5, 0x00D5}, {0x00F6, 0x00D6},
        {0x00F9, 0x00D9}, {0x00FA, 0x00DA}, {0x00FB, 0x00DB},
        {0x00FC, 0x00DC}, {0x00FD, 0x00DD}, {0x00FF, 0x0178},
        {0x0101, 0x0100}, {0x0103, 0x0102}, {0x0105, 0x0104},
        {0x0107, 0x0106}, {0x0109, 0x0108}, {0x010B, 0x010A},
        {0x010D, 0x010C}, {0x010F, 0x010E}, {0x0111, 0x0110},
        {0x0113, 0x0112}, {0x0115, 0x0114}, {0x0117, 0x0116},
        {0x0119, 0x0118}, {0x011B, 0x011A}, {0x011D, 0x011C},
        {0x011F, 0x011E}, {0x0121, 0x0120}, {0x0123, 0x0122},
        {0x0125, 0x0124}, {0x0127, 0x0126}, {0x0129, 0x0128},
        {0x012B, 0x012A}, {0x012D, 0x012C}, {0x012F, 0x012E},
        {0x0131, 0x0049}, // dotless i → I
        {0x0135, 0x0134}, {0x0137, 0x0136}, {0x013A, 0x0139},
        {0x013C, 0x013B}, {0x013E, 0x013D}, {0x0140, 0x013F},
        {0x0142, 0x0141}, {0x0144, 0x0143}, {0x0146, 0x0145},
        {0x0148, 0x0147}, {0x014B, 0x014A}, {0x014D, 0x014C},
        {0x014F, 0x014E}, {0x0151, 0x0150}, {0x0153, 0x0152},
        {0x0155, 0x0154}, {0x0157, 0x0156}, {0x0159, 0x0158},
        {0x015B, 0x015A}, {0x015D, 0x015C}, {0x015F, 0x015E},
        {0x0161, 0x0160}, {0x0163, 0x0162}, {0x0165, 0x0164},
        {0x0167, 0x0166}, {0x0169, 0x0168}, {0x016B, 0x016A},
        {0x016D, 0x016C}, {0x016F, 0x016E}, {0x0171, 0x0170},
        {0x0173, 0x0172}, {0x0175, 0x0174}, {0x0177, 0x0176},
        {0x017A, 0x0179}, {0x017C, 0x017B}, {0x017E, 0x017D},
    };
    auto it = lowerToUpper.find(codePoint);
    return it != lowerToUpper.end() ? it->second : codePoint;
}

uint32_t toLowerCase(uint32_t codePoint) {
    // ASCII
    if (codePoint >= 'A' && codePoint <= 'Z') return codePoint + 32;
    // Latin-1 Supplement uppercase to lowercase (reverse of above)
    static const std::unordered_map<uint32_t, uint32_t> upperToLower = {
        {0x00C0, 0x00E0}, {0x00C1, 0x00E1}, {0x00C2, 0x00E2},
        {0x00C3, 0x00E3}, {0x00C4, 0x00E4}, {0x00C5, 0x00E5},
        {0x00C7, 0x00E7}, {0x00C8, 0x00E8}, {0x00C9, 0x00E9},
        {0x00CA, 0x00EA}, {0x00CB, 0x00EB}, {0x00CC, 0x00EC},
        {0x00CD, 0x00ED}, {0x00CE, 0x00EE}, {0x00CF, 0x00EF},
        {0x00D1, 0x00F1}, {0x00D2, 0x00F2}, {0x00D3, 0x00F3},
        {0x00D4, 0x00F4}, {0x00D5, 0x00F5}, {0x00D6, 0x00F6},
        {0x00D9, 0x00F9}, {0x00DA, 0x00FA}, {0x00DB, 0x00FB},
        {0x00DC, 0x00FC}, {0x00DD, 0x00FD}, {0x0178, 0x00FF},
        {0x0100, 0x0101}, {0x0102, 0x0103}, {0x0104, 0x0105},
        {0x0106, 0x0107}, {0x0108, 0x0109}, {0x010A, 0x010B},
        {0x010C, 0x010D}, {0x010E, 0x010F}, {0x0110, 0x0111},
        {0x0112, 0x0113}, {0x0114, 0x0115}, {0x0116, 0x0117},
        {0x0118, 0x0119}, {0x011A, 0x011B}, {0x011C, 0x011D},
        {0x011E, 0x011F}, {0x0120, 0x0121}, {0x0122, 0x0123},
        {0x0124, 0x0125}, {0x0126, 0x0127}, {0x0128, 0x0129},
        {0x012A, 0x012B}, {0x012C, 0x012D}, {0x012E, 0x012F},
        {0x0130, 0x0069}, // İ → i
        {0x0134, 0x0135}, {0x0136, 0x0137}, {0x0139, 0x013A},
        {0x013B, 0x013C}, {0x013D, 0x013E}, {0x013F, 0x0140},
        {0x0141, 0x0142}, {0x0143, 0x0144}, {0x0145, 0x0146},
        {0x0147, 0x0148}, {0x014A, 0x014B}, {0x014C, 0x014D},
        {0x014E, 0x014F}, {0x0150, 0x0151}, {0x0152, 0x0153},
        {0x0154, 0x0155}, {0x0156, 0x0157}, {0x0158, 0x0159},
        {0x015A, 0x015B}, {0x015C, 0x015D}, {0x015E, 0x015F},
        {0x0160, 0x0161}, {0x0162, 0x0163}, {0x0164, 0x0165},
        {0x0166, 0x0167}, {0x0168, 0x0169}, {0x016A, 0x016B},
        {0x016C, 0x016D}, {0x016E, 0x016F}, {0x0170, 0x0171},
        {0x0172, 0x0173}, {0x0174, 0x0175}, {0x0176, 0x0177},
        {0x0179, 0x017A}, {0x017B, 0x017C}, {0x017D, 0x017E},
    };
    auto it = upperToLower.find(codePoint);
    return it != upperToLower.end() ? it->second : codePoint;
}

// ============================================================
// PREFIX TREE (TRIE) IMPLEMENTATION
// ============================================================

void StringPrefixTree::insert(const std::string& prefix) {
    TrieNode* node = &root_;
    for (char c : prefix) {
        node = &node->children[c];
    }
    if (!node->isEnd) {
        node->isEnd = true;
        count_++;
    }
}

bool StringPrefixTree::search(const std::string& prefix) const {
    const TrieNode* node = &root_;
    for (char c : prefix) {
        auto it = node->children.find(c);
        if (it == node->children.end()) return false;
        node = &it->second;
    }
    return node->isEnd;
}

bool StringPrefixTree::startsWith(const std::string& text) const {
    const TrieNode* node = &root_;
    for (char c : text) {
        auto it = node->children.find(c);
        if (it == node->children.end()) return false;
        node = &it->second;
    }
    return true;
}

static void collectCompletions(const StringPrefixTree::TrieNode* node,
                                const std::string& prefix,
                                std::vector<std::string>& results,
                                int maxResults) {
    if (static_cast<int>(results.size()) >= maxResults) return;
    if (node->isEnd) results.push_back(prefix);
    for (const auto& [c, child] : node->children) {
        if (static_cast<int>(results.size()) >= maxResults) return;
        collectCompletions(&child, prefix + c, results, maxResults);
    }
}

std::vector<std::string> StringPrefixTree::complete(
    const std::string& prefix, int maxResults) const {
    const TrieNode* node = &root_;
    for (char c : prefix) {
        auto it = node->children.find(c);
        if (it == node->children.end()) return {};
        node = &it->second;
    }
    std::vector<std::string> results;
    if (node->isEnd) results.push_back(prefix);
    for (const auto& [c, child] : node->children) {
        if (static_cast<int>(results.size()) >= maxResults) break;
        collectCompletions(&child, prefix + c, results, maxResults);
    }
    return results;
}

size_t StringPrefixTree::size() const { return count_; }

void StringPrefixTree::clear() {
    root_.children.clear();
    root_.isEnd = false;
    count_ = 0;
}

// ============================================================
// STRING MATCHING — exact, prefix, suffix, contains, fuzzy
// Original Kotlin:
//   Various string match utilities for search functionality
// ============================================================

bool matchString(const std::string& haystack, const std::string& needle,
                 StringMatchType type, double fuzzyThreshold) {
    switch (type) {
        case StringMatchType::EXACT:
            return haystack == needle;
        case StringMatchType::PREFIX:
            return haystack.size() >= needle.size() &&
                   haystack.compare(0, needle.size(), needle) == 0;
        case StringMatchType::SUFFIX:
            return haystack.size() >= needle.size() &&
                   haystack.compare(haystack.size() - needle.size(), needle.size(), needle) == 0;
        case StringMatchType::CONTAINS:
            return haystack.find(needle) != std::string::npos;
        case StringMatchType::FUZZY: {
            auto result = fuzzyMatch(haystack, needle);
            return result.score >= fuzzyThreshold;
        }
    }
    return false;
}

// Simple Levenshtein-based fuzzy matching with index alignment
static double levenshteinScore(const std::string& s1, const std::string& s2) {
    // Original Kotlin:
    //   Damerau-Levenshtein distance with normalized score
    size_t m = s1.size();
    size_t n = s2.size();
    if (m == 0 && n == 0) return 1.0;
    if (m == 0 || n == 0) return 0.0;

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (size_t i = 0; i <= m; ++i) dp[i][0] = static_cast<int>(i);
    for (size_t j = 0; j <= n; ++j) dp[0][j] = static_cast<int>(j);

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + cost);
            }
        }
    }
    int distance = dp[m][n];
    double maxLen = static_cast<double>(std::max(m, n));
    return 1.0 - (static_cast<double>(distance) / maxLen);
}

StringFuzzyMatch fuzzyMatch(const std::string& haystack, const std::string& needle) {
    StringFuzzyMatch result;
    result.score = 0.0;

    // Try matching needle against each substring window of equal length
    if (needle.size() <= haystack.size()) {
        for (size_t i = 0; i <= haystack.size() - needle.size(); ++i) {
            double windowScore = levenshteinScore(haystack.substr(i, needle.size()), needle);
            if (windowScore > result.score) {
                result.score = windowScore;
                // Record alignment indices
                result.alignments.clear();
                for (size_t j = 0; j < needle.size(); ++j) {
                    if (i + j < haystack.size()) result.alignments.push_back(static_cast<int>(i + j));
                }
            }
        }
    }

    // Also try matching by inserting/deleting chars
    double globalScore = levenshteinScore(haystack, needle);
    if (globalScore > result.score) {
        result.score = globalScore;
    }
    return result;
}

} // namespace progressive
