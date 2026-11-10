#include "pcp/link_preview_cache.hpp"
#include <sstream>

std::string cachePreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cachePreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getCachedPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getCachedPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isPreviewStale(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isPreviewStale"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string clearPreviewCache(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"clearPreviewCache"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
