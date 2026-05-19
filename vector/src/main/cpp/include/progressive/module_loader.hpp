#ifndef PROGRESSIVE_MODULE_LOADER_HPP
#define PROGRESSIVE_MODULE_LOADER_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace progressive {

struct ModuleInfo {
    std::string name;          // e.g. "messagecontextmenu"
    std::string soPath;        // e.g. "/data/app/.../libmessagecontextmenu.so"
    std::string version;
    bool loaded = false;
    bool enabled = false;
};

class ModuleLoader {
public:
    // Scan a directory for .so modules and register them.
    void scanDirectory(const std::string& dirPath);

    // Enable a module by name.
    void enable(const std::string& name);

    // Disable a module by name.
    void disable(const std::string& name);

    // Check if a module is enabled.
    bool isEnabled(const std::string& name) const;

    // Get module info.
    const ModuleInfo* getModule(const std::string& name) const;

    // List all registered modules as JSON.
    std::string listModulesJson() const;

    // Load all enabled modules (call on app start).
    void loadEnabled();

    // Unload all modules.
    void unloadAll();

    size_t count() const { return modules_.size(); }

private:
    std::unordered_map<std::string, ModuleInfo> modules_;
    bool validateModule(const std::string& path) const;
};

// Extract version from .so filename or embedded string
std::string extractModuleVersion(const std::string& soPath);

// Check if a .so is a valid Progressive module
bool isProgressiveModule(const std::string& soPath);

} // namespace progressive

#endif // PROGRESSIVE_MODULE_LOADER_HPP
