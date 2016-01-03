#include "Application.h"

#include "Platform/GameHub.h"
#include "FileSystem/FileSystem.h"

namespace re {

FileSystem& FileSystem::getInstance()
{
    static FileSystem instance;

    return instance;
}

Application::Application()
{
    // application = this;

    // this->view = new NativeWindow();
}

}