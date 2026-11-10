#include "pcp/auth_models.hpp"
#include "pcp/sync_models.hpp"
#include "pcp/backup_utils.hpp"
#include "pcp/media_viewer.hpp"
#include "pcp/sqlite_wrapper.hpp"
#include <string>
#include <vector>

namespace pcp {

std::string canonicalizeJson(const std::string& json) { return json; }
SyncResponse parseSyncResponse(const std::string& json) { SyncResponse sr; return sr; }
std::string getMediaTypeName(MediaType) { return "unknown"; }

SqliteDB SqliteDB::open(const std::string&) { SqliteDB db; return db; }
SqliteDB::~SqliteDB() = default;
bool SqliteDB::execute(const std::string&) { return false; }
bool SqliteDB::createTimelineSchema() { return false; }
int SqliteDB::schemaVersion() { return 0; }
void SqliteDB::setSchemaVersion(int) {}
bool SqliteDB::insertEvent(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,long,long,int,const std::string&,const std::string&,const std::string&,const std::string&) { return false; }
std::vector<SqliteDB::EventRow> SqliteDB::queryEvents(const std::string&,int,int,bool) { return {}; }

} // namespace pcp
