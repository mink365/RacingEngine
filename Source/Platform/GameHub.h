#ifndef RE_GAMEHUB_H
#define RE_GAMEHUB_H

#include "Base/Singleton.h"
#include "Math/Time.h"

#include <functional>
#include "NativeView.h"
#include "ApplicationProtocol.h"

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

    void init(ApplicationProtocol* app);

    void mainLoop(int64_t dt);
    void bindUpdateFunc(std::function<void(int64_t time)> func);

    Renderer& GetRenderer();
    TweenEngine::TweenManager &GetTweenManager();
    ApplicationProtocol& GetApp();

    const Time& GetGameTime() const;
    const Time& GetDeltaTime() const;
    int getFps();

private:
    void updateFps(int64_t dt, int &fps);

private:
    std::function<void(int64_t time)> updateFunc;
    TweenEngine::TweenManager* tweenManager;

    ApplicationProtocol* app;

    Time _gameTime;
    Time _lastSpanTime;

    int fps;
};

} // namespace re

#endif // RE_GAMEHUB_H
