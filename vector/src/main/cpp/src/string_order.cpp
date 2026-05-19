#include "progressive/string_order.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ==== Simple Big Integer Helpers (decimal strings) ====
// Used by stringToBigInt/bigIntToString for fractional indexing.
// We keep them as decimal strings to avoid external library dependency.

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
    // Assumes a >= b
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
    // Remove leading zeros
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
    // Simple long division returning integer quotient
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
        // current / base → quotient, current % base → remainder digit
        std::string quotient = divBigInt(current, baseStr);
        std::string remainder = subBigInt(current, mulBigInt(quotient, baseStr));
        int idx = std::stoi(remainder);
        if (idx >= 0 && idx < base) {
            result += alphabet[idx];
        }
        current = quotient;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ==== String Order (from StringOrderUtils.kt) ====

std::vector<std::string> stringMidPoints(
    const std::string& left,
    const std::string& right,
    int count,
    const std::string& alphabet)
{
    // Original: if (left == right) return null
    if (left == right) return {};
    if (left > right) return stringMidPoints(right, left, count, alphabet);

    // Pad to same size
    size_t size = std::max(left.size(), right.size());
    std::string leftPadded = left;
    std::string rightPadded = right;
    while (leftPadded.size() < size) leftPadded += alphabet[0];
    while (rightPadded.size() < size) rightPadded += alphabet[0];

    // Convert to big integers
    std::string b1 = stringToBigInt(leftPadded, alphabet);
    std::string b2 = stringToBigInt(rightPadded, alphabet);

    // step = (b2 - b1) / (count + 1)
    std::string diff = subBigInt(b2, b1);
    std::string step = divBigInt(diff, std::to_string(count + 1));

    std::vector<std::string> orders;
    std::string previous = left;
    std::string current = b1;

    for (int i = 0; i < count; ++i) {
        current = addBigInt(current, step);
        std::string newOrder = bigIntToString(current, alphabet);

        // Original: ensure there was enough precision
        if (previous >= newOrder) return {};

        orders.push_back(newOrder);
        previous = newOrder;
    }

    // Original: return orders.takeIf { orders.last() < right }
    if (orders.empty() || orders.back() >= right) return {};
    return orders;
}

std::string stringAverage(
    const std::string& left,
    const std::string& right,
    const std::string& alphabet)
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
    const std::string& movedItemId,
    int delta)
{
    if (delta == 0) return {};

    // Find moved item index
    int movedIndex = -1;
    for (size_t i = 0; i < orderedItemIds.size(); ++i) {
        if (orderedItemIds[i] == movedItemId) { movedIndex = static_cast<int>(i); break; }
    }
    if (movedIndex < 0) return {};

    int targetIndex = (delta > 0) ? movedIndex + delta : (movedIndex + delta - 1);

    // Collect items that need reordering (those with null orders between moved and target)
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

    // Get after-space order
    std::string afterOrder;
    int afterIdx = targetIndex + 1;
    if (afterIdx < static_cast<int>(orderedItemIds.size())) {
        afterOrder = currentOrders[afterIdx];
    }
    if (afterOrder.empty()) {
        // Default max: 4 chars of last alphabet char
        afterOrder = std::string(4, DEFAULT_ORDER_ALPHABET[93]); // ~ is last printable
    }

    if (lowerBoundOrder.empty()) {
        lowerBoundOrder = std::string(4, DEFAULT_ORDER_ALPHABET[0]); // space
    }

    // Compute new orders
    auto newOrders = stringMidPoints(lowerBoundOrder, afterOrder, static_cast<int>(nodesToReNumber.size()));

    std::vector<ReorderCommand> result;
    if (newOrders.empty()) {
        // Fallback: renumber everything
        // This is a simplified fallback
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

} // namespace progressive
