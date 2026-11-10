#ifndef PCP_TDLIB_FILE_MANAGER_HPP
#define PCP_TDLIB_FILE_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <functional>

namespace pcp {

struct FileEntry {
    int fileId = 0;
    int64_t expectedSize = 0;
    int64_t downloadedSize = 0;
    std::string localPath;
    bool isDownloading = false;
    bool isCompleted = false;
};

using FileProgressCallback = std::function<void(int fileId, int64_t downloaded, int64_t total, bool completed)>;

class TdFileManager {
public:
    TdFileManager() = default;

    void setProgressCallback(FileProgressCallback cb) { progressCb = std::move(cb); }

    void onFileUpdate(int fileId, int64_t size, int64_t downloaded, bool completed);
    void addDownload(int fileId, int64_t expectedSize = 0);
    void removeDownload(int fileId);

    float getProgress(int fileId) const;
    bool isDownloaded(int fileId) const;
    std::vector<FileEntry> activeDownloads() const;

private:
    mutable std::mutex mtx;
    std::unordered_map<int, FileEntry> files;
    FileProgressCallback progressCb;
};

} // namespace pcp
#endif
