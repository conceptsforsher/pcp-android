#pragma once

#include <vector>
#include <algorithm>
#include <cstddef>

namespace progressive {

// ================================================================
// CircularCache<T> — FIFO circular buffer (ring buffer)
//
// Ported from Element Android:
//   im.vector.app.features.notifications.CircularCache.kt (32L)
//
// Used for notification deduplication: track recently-seen event
// IDs to avoid notifying twice for the same message.
//
// NOT thread-safe. Single-threaded use only.
// ================================================================

template <typename T>
class CircularCache {
public:
    explicit CircularCache(size_t capacity)
        : buf_(capacity), writePos_(0), count_(0) {}

    // Add a value to the buffer, overwriting oldest if full.
    void put(const T& value) {
        if (writePos_ >= buf_.size()) writePos_ = 0;
        buf_[writePos_] = value;
        writePos_++;
        if (count_ < buf_.size()) count_++;
    }

    // Add a value (move semantics).
    void put(T&& value) {
        if (writePos_ >= buf_.size()) writePos_ = 0;
        buf_[writePos_] = std::move(value);
        writePos_++;
        if (count_ < buf_.size()) count_++;
    }

    // Check if value is in the buffer.
    bool contains(const T& value) const {
        for (size_t i = 0; i < count_; i++) {
            if (buf_[i] == value) return true;
        }
        return false;
    }

    // Number of elements currently stored.
    size_t size() const { return count_; }

    // Maximum capacity.
    size_t capacity() const { return buf_.size(); }

    // Whether the buffer is empty.
    bool empty() const { return count_ == 0; }

    // Whether the buffer is full.
    bool full() const { return count_ == buf_.size(); }

    // Clear all elements.
    void clear() { count_ = 0; writePos_ = 0; }

    // Access element by logical index (0 = oldest, count_-1 = newest).
    const T& operator[](size_t index) const { return buf_[index]; }

    // Iterate from oldest to newest.
    template <typename F>
    void forEach(F&& fn) const {
        for (size_t i = 0; i < count_; i++) fn(buf_[i]);
    }

private:
    std::vector<T> buf_;
    size_t writePos_;
    size_t count_;
};

} // namespace progressive
