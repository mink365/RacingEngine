#ifndef RE_APPLICATION_H
#define RE_APPLICATION_H

#include "Base/Singleton.h"
#include "Platform/ApplicationProtocol.h"

namespace re {

class Application : public ApplicationProtocol
{
public:
    Application();
};

} // namespace re

#ifdef __cplusplus
extern "C" {
#endif

void application_init(const char* resource_dir);
void application_initView(uint32_t width, uint32_t height, uint32_t framebuffer,
                            uint32_t colorRenderbuffer, uint32_t depthRenderbuffer, bool depth, bool stencil);
void application_update();
 
#ifdef __cplusplus
}
#endif

#endif // RE_APPLICATION_H