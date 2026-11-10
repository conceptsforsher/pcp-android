#include "pcp/sync_state_utils.hpp"
#include <sstream>
namespace pcp {
SyncState parseSyncState(const std::string& json) {
    SyncState s; auto p=json.find("\"next_batch\":\""); if(p!=std::string::npos){p+=14;auto e=json.find('"',p);if(e!=std::string::npos)s.nextBatch=json.substr(p,e-p);}
    return s;
}
std::string formatSyncState(const SyncState& s) {
    std::ostringstream os; os << "Syncing " << s.pendingRooms << " rooms";
    if (!s.nextBatch.empty()) os << " batch=" << s.nextBatch.substr(0,8); return os.str();
}
bool isInitialSync(const SyncState& s) { return s.nextBatch.empty(); }
}
