#include "pcp/media_progress_utils.hpp"
#include <sstream>
namespace pcp {
std::string buildMediaProgressJson(const MediaProgress& p) { std::ostringstream os; os<<R"({"url":")"<<p.mxcUrl<<R"(","total":)"<<p.totalBytes<<R"(,"downloaded":)"<<p.downloadedBytes<<"}"; return os.str(); }
MediaProgress parseMediaProgress(const std::string& json) { MediaProgress p; auto e=[&](const std::string& k)->std::string{auto pp=json.find("\""+k+"\":\"");if(pp==std::string::npos)return"";pp+=k.size()+4;auto ee=json.find('"',pp);return ee!=std::string::npos?json.substr(pp,ee-pp):"";}; p.mxcUrl=e("url"); return p; }
int getProgressPercent(const MediaProgress& p) { return p.totalBytes>0?(int)(p.downloadedBytes*100/p.totalBytes):0; }
std::string formatMediaProgress(const MediaProgress& p) { return std::to_string(getProgressPercent(p))+"%"; }
}
