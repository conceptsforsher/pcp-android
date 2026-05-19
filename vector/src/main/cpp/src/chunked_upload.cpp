#include "progressive/chunked_upload.hpp"
#include <sstream>
#include <cmath>

namespace progressive {

int ChunkedUploader::computeChunks(int64_t fileSize) {
    fileSize_ = fileSize;
    if (fileSize <= 0 || chunkSizeBytes_ <= 0) {
        totalChunks_ = 0;
        return 0;
    }
    totalChunks_ = static_cast<int>((fileSize + chunkSizeBytes_ - 1) / chunkSizeBytes_);
    currentChunk_ = 0;
    chunksSent_ = 0;
    state_ = UploadState::Idle;
    error_.clear();
    return totalChunks_;
}

ChunkInfo ChunkedUploader::getChunkInfo(int chunkIndex) const {
    ChunkInfo info;
    info.chunkIndex = chunkIndex;
    info.totalChunks = totalChunks_;
    info.totalSize = fileSize_;
    info.offset = static_cast<int64_t>(chunkIndex) * chunkSizeBytes_;
    info.chunkSize = chunkSizeBytes_;
    info.isLast = false;

    if (chunkIndex < 0 || chunkIndex >= totalChunks_) {
        info.chunkSize = 0;
        info.isLast = false;
        return info;
    }

    // Last chunk may be smaller
    int64_t remaining = fileSize_ - info.offset;
    if (remaining < chunkSizeBytes_) {
        info.chunkSize = remaining;
        info.isLast = true;
    }

    return info;
}

void ChunkedUploader::advanceChunk() {
    ++chunksSent_;
    ++currentChunk_;
    if (currentChunk_ >= totalChunks_) {
        state_ = UploadState::Done;
    } else {
        state_ = UploadState::Reading;
    }
}

void ChunkedUploader::cancel() {
    state_ = UploadState::Cancelled;
}

void ChunkedUploader::fail(const std::string& error) {
    state_ = UploadState::Failed;
    error_ = error;
}

void ChunkedUploader::reset() {
    fileSize_ = 0;
    totalChunks_ = 0;
    currentChunk_ = 0;
    chunksSent_ = 0;
    state_ = UploadState::Idle;
    error_.clear();
}

UploadProgress ChunkedUploader::progress() const {
    UploadProgress prog;
    prog.totalBytes = fileSize_;
    prog.totalChunks = totalChunks_;
    prog.chunksCompleted = chunksSent_;
    prog.done = (state_ == UploadState::Done);
    prog.cancelled = (state_ == UploadState::Cancelled);
    prog.failed = (state_ == UploadState::Failed);
    prog.error = error_;

    if (chunksSent_ > 0) {
        int lastChunk = chunksSent_ - 1;
        auto info = getChunkInfo(lastChunk);
        prog.bytesUploaded = info.offset + info.chunkSize;
        if (state_ == UploadState::Done) {
            prog.bytesUploaded = fileSize_;
        }
    }

    return prog;
}

std::string ChunkedUploader::formatContentRange(const ChunkInfo& chunk) {
    if (chunk.chunkSize == 0) return "";
    std::ostringstream header;
    header << "bytes " << chunk.offset << "-"
           << (chunk.offset + chunk.chunkSize - 1)
           << "/" << chunk.totalSize;
    return header.str();
}

std::string ChunkedUploader::chunkToJson(const ChunkInfo& chunk) {
    std::ostringstream json;
    json << "{";
    json << R"("offset": )" << chunk.offset << ",";
    json << R"("chunkSize": )" << chunk.chunkSize << ",";
    json << R"("totalSize": )" << chunk.totalSize << ",";
    json << R"("chunkIndex": )" << chunk.chunkIndex << ",";
    json << R"("totalChunks": )" << chunk.totalChunks << ",";
    json << R"("isLast": )" << (chunk.isLast ? "true" : "false");
    json << "}";
    return json.str();
}

int ChunkedUploader::suggestChunkSizeMb(int64_t fileSize) {
    if (fileSize < 100LL * 1024 * 1024) return 10;
    if (fileSize < 1024LL * 1024 * 1024) return 20;
    return 50;
}

ChunkedUploader::ListChunkSize ChunkedUploader::computeBestListChunkSize(int listSize, int limit) {
    if (listSize <= limit) return {1, listSize};
    int numberOfChunks = (listSize + limit - 1) / limit;
    int chunkSize = (listSize + numberOfChunks - 1) / numberOfChunks;
    return {numberOfChunks, chunkSize};
}

} // namespace progressive
