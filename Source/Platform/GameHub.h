#ifndef RE_GAMEHUB_H
#define RE_GAMEHUB_H

#include "Base/Singleton.h"

#include <functional>
#include "NativeView.h"

namespace TweenEngine {
    class TweenManager;
}

namespace re {

class Renderer;

class GameHub : public Singleton<GameHub>
{
public:
    GameHub();
    ~GameHub();

    void init();

    void mainLoop(long dt);
    void bindUpdateFunc(std::function<void(long time)> func);

    Renderer& GetRenderer();
    TweenEngine::TweenManager &getTweenManager();

    int getFps();
private:
    std::function<void(long time)> updateFunc;
    TweenEngine::TweenManager* tweenManager;

    int fps;
    long _time;
    int _frame;
};

} // namespace re

#endif // RE_GAMEHUB_H
