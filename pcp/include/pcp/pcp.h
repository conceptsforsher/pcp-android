#ifndef PCP_PCP_H
#define PCP_PCP_H

#include <string>

#define PCP_VERSION_MAJOR 1
#define PCP_VERSION_MINOR 0
#define PCP_VERSION_PATCH 0

namespace pcp {

struct RendererConfig;
struct Frame;
class Renderer;
class Window;

std::string version();

} // namespace pcp

#endif // PCP_PCP_H
