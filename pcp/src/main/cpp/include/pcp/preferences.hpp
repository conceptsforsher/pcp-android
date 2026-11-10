#ifndef PCP_PREFERENCES_HPP
#define PCP_PREFERENCES_HPP

#include <string>
#include <map>
#include <mutex>

namespace pcp {

class Preferences {
public:
    static Preferences& instance();

    void load(const std::string& jsonStr);
    std::string save() const;
    void clear();

    // Generic getters
    bool getBool(const std::string& key, bool def = false) const;
    int getInt(const std::string& key, int def = 0) const;
    long getLong(const std::string& key, long def = 0) const;
    std::string getString(const std::string& key, const std::string& def = "") const;

    // Generic setters
    void setBool(const std::string& key, bool val);
    void setInt(const std::string& key, int val);
    void setLong(const std::string& key, long val);
    void setString(const std::string& key, const std::string& val);

private:
    Preferences() = default;
    mutable std::mutex mu_;
    std::map<std::string, std::string> data_;
};

} // namespace pcp

#endif
