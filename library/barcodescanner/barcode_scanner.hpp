#pragma once
#include <string>
#include <functional>
#include <memory>

namespace pcp::barcodescanner {

enum class BarcodeFormat {
    QR_CODE,
    AZTEC,
    DATA_MATRIX,
    PDF417,
    CODE_39,
    CODE_93,
    CODE_128,
    EAN_8,
    EAN_13,
    UPC_A,
    UPC_E,
    ITF,
    CODABAR,
    UNKNOWN
};

struct ScanResult {
    std::string data;
    BarcodeFormat format = BarcodeFormat::UNKNOWN;
    int64_t timestamp = 0;
    bool success = false;
    std::string error;
};

struct ScannerConfig {
    bool continuousScanning = false;
    bool focusOnTap = true;
    bool flashEnabled = false;
    bool autoFocus = true;
    int scanIntervalMs = 1000;
    std::vector<BarcodeFormat> enabledFormats;
};

class BarcodeScanner {
public:
    explicit BarcodeScanner(const ScannerConfig& config = {});
    ~BarcodeScanner();

    bool start();
    void stop();
    bool isScanning() const { return m_scanning; }

    void setFlashEnabled(bool on);
    bool isFlashEnabled() const { return m_config.flashEnabled; }
    void toggleFlash();

    // Single image scan
    ScanResult scanImage(const std::vector<uint8_t>& imageData, int width, int height);

    using ScanCallback = std::function<void(const ScanResult&)>;
    void onScan(ScanCallback cb) { m_scanCallback = std::move(cb); }

    using ErrorCallback = std::function<void(const std::string&)>;
    void onError(ErrorCallback cb) { m_errorCallback = std::move(cb); }

private:
    ScannerConfig m_config;
    bool m_scanning = false;
    ScanCallback m_scanCallback;
    ErrorCallback m_errorCallback;

    std::string formatName(BarcodeFormat format) const;
};

} // namespace pcp::barcodescanner
