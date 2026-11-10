#include "pcp/sync_summary_utils.hpp"
#include <sstream>
namespace pcp {
SyncSummary parseSyncSummary(const std::string& json){ SyncSummary s; auto e=[&](const std::string& k)->std::string{auto p=json.find("\""+k+"\":\"");if(p==std::string::npos)return"";p+=k.size()+4;auto ee=json.find('"',p);return ee!=std::string::npos?json.substr(p,ee-p):"";}; s.nextBatch=e("next_batch"); return s; }
std::string formatSyncSummary(const SyncSummary& s){ std::ostringstream os; os<<s.roomsJoined<<" rooms, "<<s.totalEvents<<" events"; if(s.syncDurationMs>0)os<<" in "<<s.syncDurationMs<<"ms"; return os.str(); }
}
