#ifndef RE_GAMEHUB_H
#define RE_GAMEHUB_H

#include "Base/Singleton.h"

#include <functional>
#include "NativeView.h"

namespace re {

class Renderer;

class GameHub : public Singleton<GameHub>
{
public:
    GameHub();

    void init();

    void mainLoop(long dt);
    void bindUpdateFunc(std::function<void(long time)> func);

    Renderer& GetRenderer();
private:
    std::function<void(long time)> updateFunc;
};

} // namespace re

#endif // RE_GAMEHUB_H
