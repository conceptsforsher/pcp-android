#ifndef PROGRESSIVE_STRING_ORDER_HPP
#define PROGRESSIVE_STRING_ORDER_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- String Order / Fractional Indexing ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.util.StringOrderUtils.kt (87 lines)
//   org.matrix.android.sdk.api.session.space.SpaceOrderUtils.kt (105 lines)
//
// Implements fractional indexing for reordering items (spaces, rooms, tags)
// without renumbering everything. Similar to JIRA rank or Notion ordering.
//
// Algorithm:
//   - Uses ASCII printable alphabet (0x20-0x7E, 95 chars) as base-95
//   - Converts order strings to BigIntegers
//   - Computes midpoints via linear interpolation: step = (right - left) / (count + 1)
//   - Converts back to order strings
//   - Each new order fits lexicographically between the surrounding orders
//
// Example:
//   midPoints("a", "z", 3) → ["f", "l", "r"]  (roughly equally spaced)

// Default alphabet: ASCII printable characters 0x20-0x7E (95 chars)
constexpr const char* DEFAULT_ORDER_ALPHABET =
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

// Compute N strings lexicographically between `left` and `right`.
// Returns empty vector if there's no space (left >= right or precision exhausted).
std::vector<std::string> stringMidPoints(
    const std::string& left,
    const std::string& right,
    int count,
    const std::string& alphabet = DEFAULT_ORDER_ALPHABET);

// Compute a single midpoint between two strings (midPoints with count=1).
std::string stringAverage(
    const std::string& left,
    const std::string& right,
    const std::string& alphabet = DEFAULT_ORDER_ALPHABET);

// Convert a string to a base-N BigInteger (as decimal string).
std::string stringToBigInt(const std::string& s, const std::string& alphabet);

// Convert a decimal string (BigInteger) back to base-N string.
std::string bigIntToString(const std::string& decimal, const std::string& alphabet);

// Compute reorder commands for drag-and-drop space ordering.
// Returns minimal list of (spaceId, newOrder) to achieve the move.
struct ReorderCommand {
    std::string itemId;
    std::string newOrder;
};

std::vector<ReorderCommand> computeSpaceReorder(
    const std::vector<std::string>& orderedItemIds,
    const std::vector<std::string>& currentOrders,  // empty string = no order
    const std::string& movedItemId,
    int delta);  // positive = move down, negative = move up

} // namespace progressive

#endif // PROGRESSIVE_STRING_ORDER_HPP
