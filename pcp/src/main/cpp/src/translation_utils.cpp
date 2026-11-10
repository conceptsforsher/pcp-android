#include "pcp/translation_utils.hpp"
#include <sstream>

std::string detectLanguage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"detectLanguage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string translateText(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"translateText"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseTranslationResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseTranslationResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSupportedLanguages(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSupportedLanguages"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
