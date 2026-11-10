#include "pcp/room_favorite_utils.hpp"
#include <sstream>
namespace pcp {
std::string buildFavoriteTag(bool fav, double order) {
    std::ostringstream os; os << R"({"tags":{"m.favourite":{}})"; if(order>0)os<<R"(,"order":)"<<order; os<<"}"; return os.str();
}
std::string buildLowPriorityTag(bool lp) { return lp ? R"({"tags":{"m.lowpriority":{}}})" : R"({"tags":{}})"; }
bool isFavoritedTag(const std::string& j){return j.find("\"m.favourite\"")!=std::string::npos||j.find("\"m.favorite\"")!=std::string::npos;}
bool isLowPriorityTag(const std::string& j){return j.find("\"m.lowpriority\"")!=std::string::npos;}
}
