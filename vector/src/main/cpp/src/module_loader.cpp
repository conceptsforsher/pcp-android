#include "progressive/module_loader.hpp"
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <dlfcn.h>
#include <android/log.h>

#define LOG_TAG "ModuleLoader"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace progressive {

bool isProgressiveModule(const std::string& soPath) {
    // Check if .so exports progressive_module_init symbol
    void* handle = dlopen(soPath.c_str(), RTLD_NOW | RTLD_NOLOAD);
    if (!handle) return false;
    bool result = dlsym(handle, "progressive_module_init") != nullptr;
    dlclose(handle);
    return result;
}

std::string extractModuleVersion(const std::string& soPath) {
    void* handle = dlopen(soPath.c_str(), RTLD_NOW | RTLD_NOLOAD);
    if (!handle) return "unknown";
    auto versionFunc = reinterpret_cast<const char*(*)()>(dlsym(handle, "progressive_module_version"));
    std::string version = versionFunc ? versionFunc() : "unknown";
    dlclose(handle);
    return version;
}

void ModuleLoader::scanDirectory(const std::string& dirPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
        LOGE("Cannot open module directory: %s", dirPath.c_str());
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name(entry->d_name);
        // Look for lib*.so files that are Progressive modules
        if (name.size() > 3 && name.rfind(".so") == name.size() - 3 &&
            name.rfind("libprogressive_", 0) == 0) {
            std::string fullPath = dirPath + "/" + name;

            if (isProgressiveModule(fullPath)) {
                // Extract module name: libprogressive_messagecontextmenu.so → messagecontextmenu
                std::string moduleName = name.substr(14, name.size() - 17); // strip "libprogressive_" and ".so"
                ModuleInfo info;
                info.name = moduleName;
                info.soPath = fullPath;
                info.version = extractModuleVersion(fullPath);
                info.enabled = false; // disabled by default
                modules_[moduleName] = info;
                LOGD("Found module: %s v%s", moduleName.c_str(), info.version.c_str());
            }
        }
    }
    closedir(dir);
}

void ModuleLoader::enable(const std::string& name) {
    auto it = modules_.find(name);
    if (it != modules_.end()) {
        it->second.enabled = true;
    }
}

void ModuleLoader::disable(const std::string& name) {
    auto it = modules_.find(name);
    if (it != modules_.end()) {
        it->second.enabled = false;
    }
}

bool ModuleLoader::isEnabled(const std::string& name) const {
    auto it = modules_.find(name);
    return it != modules_.end() && it->second.enabled;
}

const ModuleInfo* ModuleLoader::getModule(const std::string& name) const {
    auto it = modules_.find(name);
    if (it != modules_.end()) return &it->second;
    return nullptr;
}

std::string ModuleLoader::listModulesJson() const {
    std::ostringstream json;
    json << "[";
    size_t i = 0;
    for (const auto& [_, m] : modules_) {
        if (i++ > 0) json << ",";
        json << R"({"name": ")" << m.name << R"(")";
        json << R"(,"version": ")" << m.version << R"(")";
        json << R"(,"enabled": )" << (m.enabled ? "true" : "false");
        json << R"(,"loaded": )" << (m.loaded ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

void ModuleLoader::loadEnabled() {
    for (auto& [_, m] : modules_) {
        if (m.enabled && !m.loaded) {
            void* handle = dlopen(m.soPath.c_str(), RTLD_NOW);
            if (handle) {
                m.loaded = true;
                LOGD("Loaded module: %s", m.name.c_str());
            } else {
                LOGE("Failed to load module %s: %s", m.name.c_str(), dlerror());
            }
        }
    }
}

void ModuleLoader::unloadAll() {
    for (auto& [_, m] : modules_) {
        if (m.loaded) {
            // Note: dlclose may not fully unload on Android due to JNI references
            m.loaded = false;
        }
    }
}

} // namespace progressive
