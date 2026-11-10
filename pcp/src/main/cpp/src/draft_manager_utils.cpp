#include "pcp/draft_manager_utils.hpp"
#include <sstream>

std::string buildDraftKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDraftKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string serializeDraft(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"serializeDraft"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDraftPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDraftPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildClearDraftContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildClearDraftContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
