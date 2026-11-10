#include "pcp/poll_vote_utils.hpp"
#include <sstream>

std::string buildPollVoteContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPollVoteContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
