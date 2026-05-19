#ifndef PROGRESSIVE_DEVICE_NAMING_HPP
#define PROGRESSIVE_DEVICE_NAMING_HPP

#include <string>

namespace progressive {

// ---- Device Naming / User Agent ----
// Ported from: org.matrix.android.sdk.internal.network.ComputeUserAgentUseCase.kt
//              (83 lines, reads Build.MANUFACTURER/MODEL/VERSION.RELEASE/DISPLAY)
//
// Original Kotlin logic:
//   1. Get app name from package manager (fallback to package name)
//   2. Get app version from package info (fallback to "0.0.0")
//   3. Read device hardware: MANUFACTURER, MODEL, Android version, Build.DISPLAY
//   4. Read SDK version from BuildConfig
//   5. Build string: "$appName/$appVersion ($manufacturer $model; Android $version; $buildId; Flavour $flavour; MatrixAndroidSdk2 $sdkVersion)"
//
// C++ port handles steps 2 and 5 (string formatting).
// Step 1 (app name), step 3 (device info), step 4 (SDK version)
// are passed as parameters from Kotlin.

// Build a Matrix user-agent string from device info.
// Original Kotlin (ComputeUserAgentUseCase.kt:execute):
//   fun execute(flavorDescription: String): String {
//       return "$appName/$appVersion ($manufacturer $model; Android $version; $buildId; Flavour $flavor; MatrixAndroidSdk2 $sdkVersion)"
//   }
std::string buildUserAgent(
    const std::string& appName,
    const std::string& appVersion,
    const std::string& manufacturer,
    const std::string& model,
    const std::string& androidVersion,
    const std::string& buildId,
    const std::string& flavorDescription,
    const std::string& sdkVersion
);

// Format a device display name for the session list.
// Original Kotlin (login_default_session_public_name string resource):
//   "${app_name} Android"
// Combined with device model: "Element Android (Pixel 7)"
std::string buildDeviceDisplayName(const std::string& appName, const std::string& deviceModel);

// Check if a string contains only ASCII characters.
// Used in the original Kotlin to validate app name.
// Original Kotlin: it.matches("\\A\\p{ASCII}*\\z".toRegex())
bool isAsciiOnly(const std::string& input);

// Format a short device identifier from manufacturer and model.
// "Google Pixel 7" → "Pixel 7"
// "samsung SM-G991B" → "SM-G991B"
// Removes manufacturer prefix if model already contains it.
std::string shortDeviceName(const std::string& manufacturer, const std::string& model);

// Format the SDK version string for user-agent.
// "1.6.10" → "MatrixAndroidSdk2 1.6.10"
std::string formatSdkVersion(const std::string& sdkVersion);

// Sanitize a component for user-agent (remove semicolons, parentheses).
// User-Agent must not contain control characters.
std::string sanitizeUserAgentComponent(const std::string& input);

} // namespace progressive

#endif // PROGRESSIVE_DEVICE_NAMING_HPP
