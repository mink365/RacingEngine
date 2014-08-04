#include "Application.h"

#include "NativeWindow.h"
#include "Platform/GameHub.h"

namespace re {

Application::Application()
{
    this->view = new NativeWindow();
}

void Application::run() {
    NativeWindow* androidWin = static_cast<NativeWindow*>(this->view);
    androidWin->initView();

    GameHub& game = GameHub::getInstance();
    game.init();

    if (!this->initEnvironment()) {
        return;
    }

    // TODO: while
}

} // namespace re
