#include "pcp/emoji_reaction_picker.hpp"
#include <sstream>

std::string getFrequentEmojis(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFrequentEmojis"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string searchEmojis(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"searchEmojis"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getEmojiCategory(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getEmojiCategory"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isCustomEmoji(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isCustomEmoji"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
