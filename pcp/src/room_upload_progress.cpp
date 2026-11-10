#include "pcp/room_upload_progress.hpp"
#include <sstream>
namespace pcp {
std::string buildUploadProgressJson(const UploadProgress& u) { std::ostringstream os; os<<R"({"path":")"<<u.localPath<<R"(","total":)"<<u.totalBytes<<R"(,"uploaded":)"<<u.uploadedBytes<<R"(,"mxc":")"<<u.mxcUrl<<R"("})"; return os.str(); }
UploadProgress parseUploadProgress(const std::string& json) { UploadProgress u; return u; }
int getUploadPercent(const UploadProgress& u) { return u.totalBytes>0?(int)(u.uploadedBytes*100/u.totalBytes):0; }
bool isUploadFailed(const UploadProgress& u) { return !u.error.empty(); }
}
