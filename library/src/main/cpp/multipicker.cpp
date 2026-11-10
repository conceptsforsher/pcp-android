/**
 * multipicker.cpp - Multi-file picker component implementation.
 *
 * Implements directory scanning, file metadata extraction, filtering,
 * sorting, selection management, thumbnail caching, and navigation.
 */

#include "multipicker.hpp"

#include <android/log.h>

#include <algorithm>
#include <cstring>
#include <sstream>

#define LOG_TAG "PcpPicker"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {
namespace library {

// ============================================================================
// Extension/category mapping tables
// ============================================================================

namespace {

const std::unordered_set<std::string> kImageExts = {
    "jpg", "jpeg", "png", "gif", "bmp", "webp", "heic", "heif",
    "svg", "tiff", "tif", "ico", "jfif", "pjpeg", "pjp", "avif"
};

const std::unordered_set<std::string> kVideoExts = {
    "mp4", "mkv", "webm", "avi", "mov", "wmv", "flv", "3gp",
    "m4v", "mpg", "mpeg", "ogv", "ts", "mts", "m2ts"
};

const std::unordered_set<std::string> kAudioExts = {
    "mp3", "wav", "ogg", "flac", "aac", "m4a", "wma", "opus",
    "amr", "aiff", "mid", "midi", "ra", "ape", "alac"
};

const std::unordered_set<std::string> kDocumentExts = {
    "pdf", "doc", "docx", "xls", "xlsx", "ppt", "pptx",
    "txt", "rtf", "csv", "odt", "ods", "odp", "pages", "numbers", "key"
};

const std::unordered_set<std::string> kArchiveExts = {
    "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "lz",
    "lz4", "zst", "ar", "cpio", "iso", "dmg", "tgz"
};

const std::unordered_set<std::string> kCodeExts = {
    "c", "cpp", "h", "hpp", "java", "kt", "py", "js", "ts",
    "html", "css", "xml", "json", "yaml", "yml", "toml", "ini",
    "rs", "go", "rb", "php", "swift", "sh", "bash", "bat", "ps1",
    "sql", "r", "m", "mm", "cs", "fs", "scala", "clj", "lua",
    "dart", "pl", "pm", "groovy", "gradle", "cmake", "makefile"
};

/** Map extension to file category. */
FileCategory extToCategory(const std::string& ext) {
    if (kImageExts.count(ext))    return FileCategory::Images;
    if (kVideoExts.count(ext))    return FileCategory::Videos;
    if (kAudioExts.count(ext))    return FileCategory::Audio;
    if (kDocumentExts.count(ext)) return FileCategory::Documents;
    if (kArchiveExts.count(ext))  return FileCategory::Archives;
    if (kCodeExts.count(ext))     return FileCategory::Code;
    if (ext == "apk" || ext == "aab") return FileCategory::APK;
    return FileCategory::All;
}

/** Map mime type to file category. */
FileCategory mimeToCategory(const std::string& mime) {
    if (mime.find("image/") != std::string::npos) return FileCategory::Images;
    if (mime.find("video/") != std::string::npos) return FileCategory::Videos;
    if (mime.find("audio/") != std::string::npos) return FileCategory::Audio;
    if (mime.find("application/pdf") != std::string::npos) return FileCategory::Documents;
    if (mime.find("application/msword") != std::string::npos) return FileCategory::Documents;
    if (mime.find("application/vnd.openxmlformats") != std::string::npos) return FileCategory::Documents;
    if (mime.find("application/vnd.android") != std::string::npos) return FileCategory::APK;
    if (mime.find("text/") != std::string::npos) return FileCategory::Documents;
    if (mime.find("application/zip") != std::string::npos) return FileCategory::Archives;
    if (mime.find("application/x-rar") != std::string::npos) return FileCategory::Archives;
    if (mime.find("application/x-7z") != std::string::npos) return FileCategory::Archives;
    return FileCategory::All;
}

/** Extract file extension (lowercase, no dot). */
std::string extractExtension(const std::string& filename) {
    auto dot = filename.rfind('.');
    if (dot == std::string::npos) return "";
    std::string ext = filename.substr(dot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

/** Check if a filename is hidden (starts with '.'). */
bool isHiddenFile(const std::string& name) {
    return !name.empty() && name[0] == '.';
}

} // anonymous namespace

// ============================================================================
// Construction / Destruction
// ============================================================================

MultiPicker::MultiPicker() {
    LOGD("MultiPicker created");
}

MultiPicker::~MultiPicker() {
    std::lock_guard<std::mutex> lock(mutex_);
    state_ = PickerState::Disposed;
    files_.clear();
    filteredFiles_.clear();
    selectedPaths_.clear();
    thumbnailCache_.clear();
    LOGD("MultiPicker destroyed");
}

// ============================================================================
// Lifecycle
// ============================================================================

bool MultiPicker::init(const PickerConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == PickerState::Disposed) {
        LOGE("init: picker already disposed");
        return false;
    }

    config_ = config;
    activeFilter_ = config.filterCategory;
    activeExtensions_ = config.allowedExtensions;
    activeSort_ = config.sortMode;

    std::string startDir = config.initialDirectory.empty()
                           ? config.rootDirectory
                           : config.initialDirectory;
    currentDirectory_ = startDir;

    state_ = PickerState::Idle;

    LOGD("MultiPicker initialized: root=%s start=%s filter=%d max=%d",
         config.rootDirectory.c_str(), startDir.c_str(),
         static_cast<int>(config.filterCategory), config.maxSelection);
    return true;
}

// ============================================================================
// Directory scanning
// ============================================================================

std::vector<FileInfo> MultiPicker::listDirectory(const std::string& dirPath) const {
    // Platform-specific: on Android, this would use MediaStore or java.io.File
    // via JNI. For now, return a documented placeholder that the real
    // implementation would fill.
    //
    // Expected JNI flow:
    //   env->FindClass("java/io/File");
    //   env->GetMethodID(..., "listFiles", "()[Ljava/io/File;");
    //   Iterate File[], build vector<FileInfo>
    //
    // This compiles but returns empty; real data comes from the Android layer.
    std::vector<FileInfo> result;

    // Attempt to scan using standard filesystem (works in tests / non-Android)
    // In production Android builds, this is replaced by JNI calls.
    LOGD("listDirectory: scanning %s", dirPath.c_str());

    // Return empty — actual implementation bridges via JNI
    return result;
}

FileInfo MultiPicker::statFile(const std::string& path) const {
    FileInfo info;
    info.path = path;

    // Extract name from path
    auto slash = path.rfind('/');
    info.name = (slash != std::string::npos) ? path.substr(slash + 1) : path;

    info.extension = extractExtension(info.name);
    info.isHidden = isHiddenFile(info.name);
    info.category = extToCategory(info.extension);
    info.isReadable = true;

    // In real implementation: stat() for size/modified
    return info;
}

bool MultiPicker::scanDirectory(const std::string& directory,
                                 FileScanCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == PickerState::Disposed) {
        LOGE("scanDirectory: picker disposed");
        return false;
    }

    state_ = PickerState::Scanning;

    std::string dir = directory.empty() ? currentDirectory_ : directory;
    currentDirectory_ = dir;

    // Validate directory is under root
    if (!config_.rootDirectory.empty() && config_.rootDirectory != "/") {
        if (dir.find(config_.rootDirectory) != 0) {
            LOGE("scanDirectory: %s is outside root %s", dir.c_str(), config_.rootDirectory.c_str());
            state_ = PickerState::Error;
            if (callback) callback(0, false);
            return false;
        }
    }

    files_ = listDirectory(dir);

    // Apply current filters
    filteredFiles_.clear();
    for (const auto& f : files_) {
        if (passesFilter(f)) {
            filteredFiles_.push_back(f);
        }
    }

    // Apply sort
    applySort();

    state_ = PickerState::Ready;

    int total = static_cast<int>(filteredFiles_.size());
    LOGD("scanDirectory: %d files in %s", total, dir.c_str());

    if (callback) {
        callback(total, true);
    }

    return true;
}

bool MultiPicker::setRootDirectory(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_.rootDirectory = path;
    // Reset to root
    currentDirectory_ = path;
    selectedPaths_.clear();
    LOGD("Root directory set to: %s", path.c_str());
    return true;
}

std::string MultiPicker::getCurrentDirectory() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return currentDirectory_;
}

bool MultiPicker::navigateUp() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (currentDirectory_ == config_.rootDirectory || currentDirectory_ == "/") {
        return false;
    }

    auto slash = currentDirectory_.rfind('/');
    if (slash == std::string::npos || slash == 0) {
        currentDirectory_ = config_.rootDirectory.empty() ? "/" : config_.rootDirectory;
    } else {
        currentDirectory_ = currentDirectory_.substr(0, slash);
    }

    LOGD("Navigated up to: %s", currentDirectory_.c_str());
    return scanDirectory();
}

bool MultiPicker::navigateTo(int index) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (index < 0 || index >= static_cast<int>(filteredFiles_.size())) {
        return false;
    }

    const auto& file = filteredFiles_[index];
    if (!file.isDirectory) return false;
    if (!config_.allowDirectories && file.isDirectory) {
        // Navigate into directory (browsing) — always allowed
        // Selection of directories is controlled by allowDirectories
    }

    std::string newPath = file.path;
    currentDirectory_ = newPath;
    return scanDirectory();
}

bool MultiPicker::navigateTo(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Validate path is under root
    if (!config_.rootDirectory.empty() && config_.rootDirectory != "/") {
        if (path.find(config_.rootDirectory) != 0) {
            LOGE("navigateTo: %s outside root", path.c_str());
            return false;
        }
    }

    currentDirectory_ = path;
    return scanDirectory();
}

bool MultiPicker::refreshDirectory() {
    return scanDirectory();
}

// ============================================================================
// Filtering
// ============================================================================

bool MultiPicker::filterByType(FileCategory category) {
    std::lock_guard<std::mutex> lock(mutex_);

    activeFilter_ = category;
    activeExtensions_.clear();

    filteredFiles_.clear();
    for (const auto& f : files_) {
        if (passesFilter(f)) {
            filteredFiles_.push_back(f);
        }
    }

    applySort();
    LOGD("Filtered by type %d: %zu files", static_cast<int>(category), filteredFiles_.size());
    return true;
}

bool MultiPicker::filterByExtension(const std::vector<std::string>& extensions) {
    std::lock_guard<std::mutex> lock(mutex_);

    activeFilter_ = FileCategory::Custom;
    activeExtensions_ = extensions;

    // Normalize to lowercase
    for (auto& ext : activeExtensions_) {
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    }

    filteredFiles_.clear();
    for (const auto& f : files_) {
        if (passesFilter(f)) {
            filteredFiles_.push_back(f);
        }
    }

    applySort();
    LOGD("Filtered by extensions: %zu files", filteredFiles_.size());
    return true;
}

// ============================================================================
// Selection
// ============================================================================

bool MultiPicker::toggleSelection(int index) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (index < 0 || index >= static_cast<int>(filteredFiles_.size())) {
        return false;
    }

    const auto& file = filteredFiles_[index];

    // Don't allow selecting directories unless configured
    if (file.isDirectory && !config_.allowDirectories) {
        return false;
    }

    std::string key = file.path.empty() ? file.uri : file.path;

    if (selectedPaths_.count(key)) {
        selectedPaths_.erase(key);
    } else {
        // Check max selection
        if (config_.maxSelection > 0 &&
            static_cast<int>(selectedPaths_.size()) >= config_.maxSelection) {
            LOGW("Max selection (%d) reached", config_.maxSelection);
            return false;
        }
        selectedPaths_.insert(key);
    }

    notifySelectionChanged();
    LOGD("Toggled selection: %s (total: %zu)", file.name.c_str(), selectedPaths_.size());
    return true;
}

bool MultiPicker::selectAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    for (const auto& file : filteredFiles_) {
        if (file.isDirectory && !config_.allowDirectories) continue;

        std::string key = file.path.empty() ? file.uri : file.path;

        if (config_.maxSelection > 0 &&
            static_cast<int>(selectedPaths_.size()) >= config_.maxSelection) {
            break;
        }
        selectedPaths_.insert(key);
    }

    notifySelectionChanged();
    LOGD("Selected all: %zu files", selectedPaths_.size());
    return true;
}

bool MultiPicker::deselectAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    selectedPaths_.clear();
    notifySelectionChanged();
    LOGD("All deselected");
    return true;
}

std::vector<FileInfo> MultiPicker::getSelectedFiles() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<FileInfo> result;
    for (const auto& file : files_) {
        std::string key = file.path.empty() ? file.uri : file.path;
        if (selectedPaths_.count(key)) {
            result.push_back(file);
        }
    }
    // Also check filtered files that might not be in files_ (shouldn't happen normally)
    for (const auto& file : filteredFiles_) {
        std::string key = file.path.empty() ? file.uri : file.path;
        if (selectedPaths_.count(key)) {
            // Avoid duplicates
            bool already = false;
            for (const auto& r : result) {
                if (r == file) { already = true; break; }
            }
            if (!already) result.push_back(file);
        }
    }
    return result;
}

int MultiPicker::getSelectionCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(selectedPaths_.size());
}

void MultiPicker::setMaxSelection(int max) {
    std::lock_guard<std::mutex> lock(mutex_);
    config_.maxSelection = max;
    LOGD("Max selection set to: %d", max);
}

// ============================================================================
// File list access
// ============================================================================

int MultiPicker::getFileCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(filteredFiles_.size());
}

FileInfo MultiPicker::getFileAt(int index) const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (index < 0 || index >= static_cast<int>(filteredFiles_.size())) {
        return FileInfo{};
    }
    return filteredFiles_[index];
}

// ============================================================================
// Thumbnails
// ============================================================================

std::string MultiPicker::getThumbnail(int index) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (index < 0 || index >= static_cast<int>(filteredFiles_.size())) {
        return "";
    }

    const auto& file = filteredFiles_[index];
    std::string key = file.path.empty() ? file.uri : file.path;

    // Check cache
    auto cacheIt = thumbnailCache_.find(key);
    if (cacheIt != thumbnailCache_.end()) {
        return cacheIt->second;
    }

    // Check if the file already has a thumbnail path
    if (!file.thumbnailPath.empty()) {
        thumbnailCache_[key] = file.thumbnailPath;
        return file.thumbnailPath;
    }

    // In production: request thumbnail generation via JNI
    // For images: use MediaStore.Images.Thumbnails
    // For videos: use MediaStore.Video.Thumbnails
    // For documents: use PdfRenderer or similar
    LOGD("getThumbnail: no cached thumbnail for %s", file.name.c_str());

    if (thumbnailCallback_) {
        thumbnailCallback_(index, "");
    }

    return "";
}

// ============================================================================
// Sorting
// ============================================================================

void MultiPicker::sortByName(bool ascending) {
    std::lock_guard<std::mutex> lock(mutex_);
    activeSort_ = ascending ? SortMode::NameAsc : SortMode::NameDesc;
    applySort();
}

void MultiPicker::sortByDate(bool ascending) {
    std::lock_guard<std::mutex> lock(mutex_);
    activeSort_ = ascending ? SortMode::DateAsc : SortMode::DateDesc;
    applySort();
}

void MultiPicker::sortBySize(bool ascending) {
    std::lock_guard<std::mutex> lock(mutex_);
    activeSort_ = ascending ? SortMode::SizeAsc : SortMode::SizeDesc;
    applySort();
}

// ============================================================================
// URI / Path helpers
// ============================================================================

std::vector<std::string> MultiPicker::getSelectedPaths() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::string> paths;
    auto selected = getSelectedFiles();
    for (const auto& f : selected) {
        if (!f.path.empty()) paths.push_back(f.path);
    }
    return paths;
}

std::vector<std::string> MultiPicker::getSelectedUris() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::string> uris;
    auto selected = getSelectedFiles();
    for (const auto& f : selected) {
        if (!f.uri.empty()) {
            uris.push_back(f.uri);
        } else if (!f.path.empty()) {
            // Convert file path to URI (simplified)
            uris.push_back("file://" + f.path);
        }
    }
    return uris;
}

bool MultiPicker::isValidFile(int index) const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (index < 0 || index >= static_cast<int>(filteredFiles_.size())) {
        return false;
    }
    return filteredFiles_[index].isReadable;
}

// ============================================================================
// Interaction
// ============================================================================

bool MultiPicker::onLongPress(int index) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!config_.selectOnLongPress) return false;

    // Toggle selection on long press
    return toggleSelection(index);
}

void MultiPicker::cancel() {
    std::lock_guard<std::mutex> lock(mutex_);

    selectedPaths_.clear();
    currentDirectory_ = config_.rootDirectory;
    state_ = PickerState::Idle;
    filteredFiles_.clear();
    LOGD("Picker cancelled");
}

void MultiPicker::setOnSelectionChanged(SelectionCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    selectionCallback_ = std::move(callback);
}

void MultiPicker::setOnScanProgress(FileScanCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    scanCallback_ = std::move(callback);
}

void MultiPicker::setOnThumbnailReady(ThumbnailCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    thumbnailCallback_ = std::move(callback);
}

// ============================================================================
// State
// ============================================================================

PickerState MultiPicker::getState() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return state_;
}

// ============================================================================
// Private helpers
// ============================================================================

FileCategory MultiPicker::categorizeFile(const std::string& extension,
                                           const std::string& mimeType) const {
    if (!extension.empty()) {
        FileCategory cat = extToCategory(extension);
        if (cat != FileCategory::All) return cat;
    }
    if (!mimeType.empty()) {
        return mimeToCategory(mimeType);
    }
    return FileCategory::All;
}

bool MultiPicker::passesFilter(const FileInfo& file) const {
    // Hidden file check
    if (file.isHidden && !config_.showHiddenFiles) {
        return false;
    }

    // Size check
    if (config_.maxFileSize > 0 && file.size > config_.maxFileSize && !file.isDirectory) {
        return false;
    }

    // Excluded paths
    for (const auto& excluded : config_.excludedPaths) {
        if (file.path.find(excluded) == 0) {
            return false;
        }
    }

    // Category filter
    if (activeFilter_ == FileCategory::Custom) {
        // Extension-based filter
        if (activeExtensions_.empty()) return true;
        for (const auto& ext : activeExtensions_) {
            if (file.extension == ext) return true;
        }
        // Directories always pass extension filters
        return file.isDirectory;
    }

    if (activeFilter_ == FileCategory::All) return true;

    // Directory check: show dirs unless specifically filtering them out
    if (file.isDirectory) return true;

    return file.category == activeFilter_;
}

void MultiPicker::applySort() {
    auto comparator = [this](const FileInfo& a, const FileInfo& b) -> bool {
        // Directories always first
        if (a.isDirectory != b.isDirectory) return a.isDirectory;

        switch (activeSort_) {
        case SortMode::NameAsc:  return a.name < b.name;
        case SortMode::NameDesc: return a.name > b.name;
        case SortMode::DateAsc:  return a.modified < b.modified;
        case SortMode::DateDesc: return a.modified > b.modified;
        case SortMode::SizeAsc:  return a.size < b.size;
        case SortMode::SizeDesc: return a.size > b.size;
        case SortMode::TypeAsc:  return a.extension < b.extension;
        case SortMode::TypeDesc: return a.extension > b.extension;
        default: return a.name < b.name;
        }
    };

    std::sort(filteredFiles_.begin(), filteredFiles_.end(), comparator);
}

void MultiPicker::resetSelection() {
    selectedPaths_.clear();
}

bool MultiPicker::isSelected(const FileInfo& file) const {
    std::string key = file.path.empty() ? file.uri : file.path;
    return selectedPaths_.count(key) > 0;
}

void MultiPicker::notifySelectionChanged() {
    if (selectionCallback_) {
        selectionCallback_(static_cast<int>(selectedPaths_.size()));
    }
}

} // namespace library
} // namespace pcp
