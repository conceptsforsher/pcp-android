#ifndef PROGRESSIVE_READ_RECEIPTS_HPP
#define PROGRESSIVE_READ_RECEIPTS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct ReceiptEntry {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    int64_t timestamp = 0;  // most recent read time
};

struct ReceiptDisplay {
    std::vector<ReceiptEntry> visibleEntries;  // avatars to show
    int overflowCount = 0;                     // how many more (0 = no "+N")
    int totalCount = 0;                        // total unique readers
    std::string accessibilityText;             // "Alice, Bob, and 5 others read"
    int maxVisible = 20;                       // configurable limit
};

// Compute the display data for read receipts.
// Sorts by timestamp (most recent first), limits to maxVisible,
// and computes the overflow "+N" and accessibility text.
ReceiptDisplay computeReceiptDisplay(
    const std::vector<ReceiptEntry>& allReceipts,
    int maxVisible
);

// Deduplicate receipts: keep only the most recent per userId.
std::vector<ReceiptEntry> deduplicateReceipts(
    const std::vector<ReceiptEntry>& receipts
);

// Format accessibility text for read receipts.
// e.g. "Alice, Bob, and 3 others read"
std::string formatReceiptAccessibility(
    const std::vector<ReceiptEntry>& visibleEntries,
    int overflowCount
);

// Format the "+N" overflow label.
std::string formatOverflowLabel(int count);

// Export receipt display data as JSON for the Kotlin UI layer.
std::string receiptDisplayToJson(const ReceiptDisplay& display);

// Sort receipts by timestamp descending (most recent first).
void sortByTimestampDesc(std::vector<ReceiptEntry>& receipts);

} // namespace progressive

#endif // PROGRESSIVE_READ_RECEIPTS_HPP
