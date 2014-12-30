#include "Application.h"

#include <unistd.h>
#include <sys/time.h>

#include "Platform/Base/desktop/NativeWindow.h"
#include "Platform/GameHub.h"
#include "FileSystem/FileSystem.h"

namespace re {

static int64_t getCurrentMillSecond() {
    int64_t lLastTime;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

FileSystem& FileSystem::getInstance()
{
    static FileSystem instance;
    return instance;
}

Application::Application()
{
    this->view = new NativeWindow();
}

void Application::run()
{
    this->initViewSize();

    NativeWindow* glfwWin = static_cast<NativeWindow*>(this->view);
    glfwWin->initView();
    glfwWin->bindEventHandler();

    GameHub& game = GameHub::getInstance();
    game.init();

    if (!this->initEnvironment()) {
        return;
    }

    int64_t lastTime = getCurrentMillSecond();
    int64_t curTime, dt, oldTime;
    int64_t _animationInterval = 1 / 60.0 * 1000;
    while(!glfwWin->shouldClose()) {

        oldTime = lastTime;
        lastTime = getCurrentMillSecond();
        dt = lastTime - oldTime;

        // main loop and draw
        game.mainLoop(dt);

        glfwWin->swapBuffers();
        glfwWin->pollEvents();

        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _animationInterval)
        {
            usleep((_animationInterval - curTime + lastTime)*1000);
        }
    }

    // end and release
}

} // namespace re
