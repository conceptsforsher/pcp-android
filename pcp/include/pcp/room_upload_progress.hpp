#pragma once
#include <string>
#include <cstdint>
namespace pcp {
struct UploadProgress { std::string localPath; int64_t totalBytes=0; int64_t uploadedBytes=0; std::string mxcUrl; bool isComplete=false; std::string error; };
std::string buildUploadProgressJson(const UploadProgress& u);
UploadProgress parseUploadProgress(const std::string& json);
int getUploadPercent(const UploadProgress& u);
bool isUploadFailed(const UploadProgress& u);
}
