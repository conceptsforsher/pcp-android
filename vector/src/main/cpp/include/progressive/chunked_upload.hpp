#ifndef PROGRESSIVE_CHUNKED_UPLOAD_HPP
#define PROGRESSIVE_CHUNKED_UPLOAD_HPP

#include <string>
#include <cstdint>
#include <functional>

namespace progressive {

struct ChunkInfo {
    int64_t offset = 0;       // byte offset of this chunk in the file
    int64_t chunkSize = 0;     // actual bytes in this chunk (may be < maxChunkSize for last chunk)
    int64_t totalSize = 0;     // total file size
    int chunkIndex = 0;        // 0-based chunk number
    int totalChunks = 0;       // total number of chunks
    bool isLast = false;       // last chunk?
};

struct UploadProgress {
    int64_t bytesUploaded = 0;
    int64_t totalBytes = 0;
    int chunksCompleted = 0;
    int totalChunks = 0;
    bool done = false;
    bool cancelled = false;
    bool failed = false;
    std::string error;
    float progress() const {
        if (totalBytes == 0) return 0.0f;
        return static_cast<float>(bytesUploaded) / static_cast<float>(totalBytes);
    }
};

enum class UploadState { Idle, Reading, Sending, Done, Cancelled, Failed };

class ChunkedUploader {
public:
    // Configure chunk size in megabytes.
    void setChunkSizeMb(int mb) { chunkSizeBytes_ = static_cast<int64_t>(mb) * 1024 * 1024; }

    // Compute chunk metadata for a file of given size.
    // Returns number of chunks.
    int computeChunks(int64_t fileSize);

    // Get info for a specific chunk (0-based).
    ChunkInfo getChunkInfo(int chunkIndex) const;

    // Total number of chunks computed.
    int totalChunks() const { return totalChunks_; }

    // Total file size.
    int64_t fileSize() const { return fileSize_; }

    // Mark current chunk as sent successfully.
    void advanceChunk();

    // Mark upload as cancelled.
    void cancel();

    // Mark upload as failed with error.
    void fail(const std::string& error);

    // Reset for a new upload.
    void reset();

    // Current state.
    UploadState state() const { return state_; }

    // Get current progress.
    UploadProgress progress() const;

    // Format a Content-Range header for Matrix chunked upload.
    // e.g. "bytes 0-10485759/52428800"
    static std::string formatContentRange(const ChunkInfo& chunk);

    // Format chunk info as JSON.
    static std::string chunkToJson(const ChunkInfo& chunk);

    // Compute optimal chunk size based on file size.
    // Files < 100MB: 10MB chunks, 100MB-1GB: 20MB, >1GB: 50MB
    static int suggestChunkSizeMb(int64_t fileSize);

    // Compute optimal chunk count and size for splitting a list.
    // Faithful port from org.matrix.android.sdk.internal.util.BestChunkSize.kt (44L)
    // Original Kotlin:
    //   fun computeBestChunkSize(listSize: Int, limit: Int): BestChunkSize {
    //       if (listSize <= limit) return BestChunkSize(1, listSize)
    //       val numberOfChunks = ceil(listSize / limit.toDouble()).toInt()
    //       val chunkSize = ceil(listSize / numberOfChunks.toDouble()).toInt()
    //       return BestChunkSize(numberOfChunks, chunkSize)
    //   }
    struct ListChunkSize { int numberOfChunks; int chunkSize; bool shouldChunk() const { return numberOfChunks > 1; } };
    static ListChunkSize computeBestListChunkSize(int listSize, int limit);

private:
    int64_t chunkSizeBytes_ = 10 * 1024 * 1024; // default 10MB
    int64_t fileSize_ = 0;
    int totalChunks_ = 0;
    int currentChunk_ = 0;
    int chunksSent_ = 0;
    UploadState state_ = UploadState::Idle;
    std::string error_;
};

} // namespace progressive

#endif // PROGRESSIVE_CHUNKED_UPLOAD_HPP
