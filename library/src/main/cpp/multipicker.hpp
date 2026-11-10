#pragma once

/**
 * multipicker.hpp - Multi-file picker component for PCP.
 *
 * Provides file browsing, selection, filtering, sorting, and thumbnail
 * generation. Designed for Android's content:// and file:// URIs.
 * Thread-safe via internal mutex.
 */

#include "core_utils.hpp"

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <unordered_set>
#include <algorithm>

namespace pcp {
namespace library {

// ============================================================================
// Enums
// ============================================================================

/** Picker lifecycle/UI state. */
enum class PickerState {
    Idle = 0,
    Scanning,          // Currently reading directory contents
    Ready,             // Directory loaded and ready for interaction
    LoadingThumbnails, // Generating thumbnail previews
    Error,             // Scan failed or directory not accessible
    Disposed           // Picker destroyed
};

/** File type categories for filtering. */
enum class FileCategory {
    All = 0,
    Images,
    Videos,
    Audio,
    Documents,
    Archives,
    Code,
    APK,
    Custom            // User-defined extension filter
};

/** Sort mode for the file list. */
enum class SortMode {
    NameAsc = 0,
    NameDesc,
    DateAsc,
    DateDesc,
    SizeAsc,
    SizeDesc,
    TypeAsc,
    TypeDesc
};

// ============================================================================
// Structs
// ============================================================================

/** Metadata for a single file. */
struct FileInfo {
    std::string name;                 // Display name (filename only)
    std::string path;                 // Absolute path
    std::string uri;                  // Content URI (Android content:// scheme)
    std::string mimeType;
    std::string extension;            // Lowercase, no dot
    int64_t size = 0;                // Bytes
    int64_t modified = 0;            // Unix timestamp (seconds)
    int64_t added = 0;               // When added to media store
    bool isDirectory = false;
    bool isHidden = false;
    bool isReadable = true;
    FileCategory category = FileCategory::All;
    int width = 0;                   // For images/video (0 if unknown)
    int height = 0;
    int64_t durationMs = 0;          // For audio/video
    std::string thumbnailPath;       // Cached thumbnail path

    /** Human-readable file size string. */
    std::string formattedSize() const {
        if (size < 1024) return std::to_string(size) + " B";
        if (size < 1024 * 1024) return std::to_string(size / 1024) + " KB";
        if (size < 1024 * 1024 * 1024) return std::to_string(size / (1024 * 1024)) + " MB";
        return std::to_string(size / (1024LL * 1024 * 1024)) + " GB";
    }

    bool operator==(const FileInfo& other) const {
        return path == other.path || uri == other.uri;
    }
    bool operator!=(const FileInfo& other) const { return !(*this == other); }
};

/** Configuration for the file picker. */
struct PickerConfig {
    std::string rootDirectory = "/";         // Starting directory
    std::string initialDirectory;            // May differ from root
    int maxSelection = 0;                     // 0 = unlimited
    FileCategory filterCategory = FileCategory::All;
    std::vector<std::string> allowedExtensions; // E.g., {"jpg", "png", "pdf"}
    bool showHiddenFiles = false;
    bool allowDirectories = false;           // Can directories be selected?
    bool showThumbnails = true;
    int thumbnailSize = 120;                 // px
    SortMode sortMode = SortMode::NameAsc;
    bool recursiveScan = false;
    int maxScanDepth = 3;                    // For recursive scans
    int64_t maxFileSize = 0;                 // 0 = unlimited
    std::vector<std::string> excludedPaths;  // Paths to skip
    bool selectOnLongPress = true;
    bool singleTapSelect = false;            // True = tap toggles selection
    Color selectionColor{103, 80, 164, 80};  // Selection highlight color
};

// ============================================================================
// Callback types
// ============================================================================

using FileScanCallback = std::function<void(int totalFiles, bool complete)>;
using SelectionCallback = std::function<void(int selectedCount)>;
using ThumbnailCallback = std::function<void(int index, const std::string& thumbnailPath)>;

// ============================================================================
// MultiPicker class
// ============================================================================

/**
 * Multi-file picker with directory browsing, selection, filtering, and sorting.
 *
 * Features:
 * - Directory navigation (up, into, arbitrary paths)
 * - Multi-selection with toggling
 * - Filter by file type or extension
 * - Sort by name, date, size, type
 * - Thumbnail generation and prefetching
 * - Max selection enforcement
 * - Long-press / tap selection modes
 *
 * All public methods are thread-safe.
 */
class MultiPicker {
public:
    MultiPicker();
    ~MultiPicker();

    // --- Lifecycle ---

    /** Initialize with configuration. */
    bool init(const PickerConfig& config = PickerConfig());

    // --- Directory operations ---

    /** Scan a directory and populate the file list. Empty string = use current. */
    bool scanDirectory(const std::string& directory = "",
                       FileScanCallback callback = nullptr);

    /** Set the root directory (top-level constraint). */
    bool setRootDirectory(const std::string& path);

    /** Get the current directory path. */
    std::string getCurrentDirectory() const;

    /** Navigate up one directory level. Returns false if at root. */
    bool navigateUp();

    /** Navigate into a subdirectory by index or path. */
    bool navigateTo(int index);
    bool navigateTo(const std::string& path);

    /** Refresh the current directory (re-scan). */
    bool refreshDirectory();

    // --- Filtering ---

    /** Filter the file list by file category. */
    bool filterByType(FileCategory category);

    /** Filter by a list of allowed extensions (without dots). */
    bool filterByExtension(const std::vector<std::string>& extensions);

    // --- Selection ---

    /** Toggle selection for a file at the given index. */
    bool toggleSelection(int index);

    /** Select all currently visible files. */
    bool selectAll();

    /** Deselect all selections. */
    bool deselectAll();

    /** Get the list of currently selected FileInfo objects. */
    std::vector<FileInfo> getSelectedFiles() const;

    /** Get the number of selected files. */
    int getSelectionCount() const;

    /** Set max selection count (0 = unlimited). */
    void setMaxSelection(int max);

    // --- File list access ---

    /** Get the total number of files in the current view. */
    int getFileCount() const;

    /** Get the FileInfo at the given index. Returns empty FileInfo if out of bounds. */
    FileInfo getFileAt(int index) const;

    // --- Thumbnails ---

    /** Request a thumbnail for the file at the given index. */
    std::string getThumbnail(int index);

    // --- Sorting ---

    /** Sort the current file list by name. */
    void sortByName(bool ascending = true);

    /** Sort the current file list by modification date. */
    void sortByDate(bool ascending = true);

    /** Sort the current file list by file size. */
    void sortBySize(bool ascending = true);

    // --- URI / Path helpers ---

    /** Get full paths of selected files. */
    std::vector<std::string> getSelectedPaths() const;

    /** Get content URIs of selected files. */
    std::vector<std::string> getSelectedUris() const;

    /** Check if a file at the given index is valid/accessible. */
    bool isValidFile(int index) const;

    // --- Interaction ---

    /** Handle a long-press on a file at the given index. Toggles selection. */
    bool onLongPress(int index);

    /** Cancel / abort the picker. Clears selection and resets to root. */
    void cancel();

    /** Set selection change callback. */
    void setOnSelectionChanged(SelectionCallback callback);

    /** Set scan progress callback. */
    void setOnScanProgress(FileScanCallback callback);

    /** Set thumbnail ready callback. */
    void setOnThumbnailReady(ThumbnailCallback callback);

    // --- State ---

    /** Get the current picker state. */
    PickerState getState() const;

private:
    // --- Internal helpers ---
    std::vector<FileInfo> listDirectory(const std::string& dirPath) const;
    FileInfo statFile(const std::string& path) const;
    FileCategory categorizeFile(const std::string& extension, const std::string& mimeType) const;
    bool passesFilter(const FileInfo& file) const;
    void applySort();
    void resetSelection();
    bool isSelected(const FileInfo& file) const;
    void notifySelectionChanged();

    // --- Thread safety ---
    mutable std::mutex mutex_;

    // --- State ---
    PickerState state_ = PickerState::Idle;
    PickerConfig config_;

    std::string currentDirectory_;
    std::vector<FileInfo> files_;          // All files in current dir
    std::vector<FileInfo> filteredFiles_;  // After filter applied
    std::unordered_set<std::string> selectedPaths_; // Selected file paths
    FileCategory activeFilter_ = FileCategory::All;
    std::vector<std::string> activeExtensions_;
    SortMode activeSort_ = SortMode::NameAsc;

    // --- Callbacks ---
    SelectionCallback selectionCallback_;
    FileScanCallback scanCallback_;
    ThumbnailCallback thumbnailCallback_;

    // --- Thumbnail cache ---
    std::unordered_map<std::string, std::string> thumbnailCache_;
};

} // namespace library
} // namespace pcp
