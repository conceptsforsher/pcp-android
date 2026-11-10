#include "pcp/sso_provider_utils.hpp"
#include <sstream>

std::string getProviders(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getProviders"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string selectProvider(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"selectProvider"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseProviderBrand(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseProviderBrand"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSsoUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSsoUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
