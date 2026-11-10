#include "pcp/tdlib_file_manager.hpp"
#include <algorithm>

namespace pcp {

void TdFileManager::onFileUpdate(int fileId, int64_t size, int64_t downloaded, bool completed) {
    std::lock_guard<std::mutex> lock(mtx);
    auto& entry = files[fileId];
    entry.fileId = fileId;
    entry.expectedSize = size;
    entry.downloadedSize = downloaded;
    entry.isDownloading = !completed;
    entry.isCompleted = completed;
    if (progressCb) progressCb(fileId, downloaded, size, completed);
}

void TdFileManager::addDownload(int fileId, int64_t expectedSize) {
    std::lock_guard<std::mutex> lock(mtx);
    files[fileId] = FileEntry{fileId, expectedSize, 0, "", true, false};
}

void TdFileManager::removeDownload(int fileId) {
    std::lock_guard<std::mutex> lock(mtx);
    files.erase(fileId);
}

float TdFileManager::getProgress(int fileId) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = files.find(fileId);
    if (it == files.end() || it->second.expectedSize == 0) return 0.0f;
    return (float)it->second.downloadedSize / (float)it->second.expectedSize;
}

bool TdFileManager::isDownloaded(int fileId) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = files.find(fileId);
    return it != files.end() && it->second.isCompleted;
}

std::vector<FileEntry> TdFileManager::activeDownloads() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<FileEntry> result;
    for (auto& kv : files) {
        if (kv.second.isDownloading) result.push_back(kv.second);
    }
    return result;
}

} // namespace pcp
