#ifndef RE_APPLICATION_H
#define RE_APPLICATION_H

#include "Base/Singleton.h"
#include "Platform/ApplicationProtocol.h"

namespace re {

class Application : public ApplicationProtocol
{
public:
    Application();

    void run();
};

} // namespace re

#endif // RE_APPLICATION_H