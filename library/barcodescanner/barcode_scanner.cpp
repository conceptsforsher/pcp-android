#include "barcode_scanner.hpp"
#include <android/log.h>

#define BS_LOG_TAG "BarcodeScanner"
#define BS_LOGI(...) __android_log_print(ANDROID_LOG_INFO, BS_LOG_TAG, __VA_ARGS__)
#define BS_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, BS_LOG_TAG, __VA_ARGS__)

namespace pcp::barcodescanner {

BarcodeScanner::BarcodeScanner(const ScannerConfig& config) : m_config(config) {
    if (m_config.enabledFormats.empty()) {
        m_config.enabledFormats = {BarcodeFormat::QR_CODE};
    }
}

BarcodeScanner::~BarcodeScanner() { stop(); }

bool BarcodeScanner::start() {
    m_scanning = true;
    BS_LOGI("Scanner started with %zu formats", m_config.enabledFormats.size());
    return true;
}

void BarcodeScanner::stop() {
    m_scanning = false;
}

void BarcodeScanner::setFlashEnabled(bool on) {
    m_config.flashEnabled = on;
}

void BarcodeScanner::toggleFlash() {
    m_config.flashEnabled = !m_config.flashEnabled;
}

ScanResult BarcodeScanner::scanImage(const std::vector<uint8_t>& imageData,
                                       int width, int height) {
    ScanResult result;
    // In production: use ZXing C++ library or Google ML Kit
    // For now, stub with proper structure
    BS_LOGI("Scanning image: %dx%d, %zu bytes", width, height, imageData.size());
    result.success = false;
    return result;
}

std::string BarcodeScanner::formatName(BarcodeFormat format) const {
    switch (format) {
        case BarcodeFormat::QR_CODE: return "QR_CODE";
        case BarcodeFormat::AZTEC: return "AZTEC";
        case BarcodeFormat::DATA_MATRIX: return "DATA_MATRIX";
        case BarcodeFormat::PDF417: return "PDF417";
        case BarcodeFormat::CODE_39: return "CODE_39";
        case BarcodeFormat::CODE_93: return "CODE_93";
        case BarcodeFormat::CODE_128: return "CODE_128";
        case BarcodeFormat::EAN_8: return "EAN_8";
        case BarcodeFormat::EAN_13: return "EAN_13";
        case BarcodeFormat::UPC_A: return "UPC_A";
        case BarcodeFormat::UPC_E: return "UPC_E";
        case BarcodeFormat::ITF: return "ITF";
        case BarcodeFormat::CODABAR: return "CODABAR";
        default: return "UNKNOWN";
    }
}

} // namespace pcp::barcodescanner
