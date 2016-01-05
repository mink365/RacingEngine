#include "Application.h"

#include "NativeWindow.h"

#include "Platform/GameHub.h"
#include "FileSystem/FileSystem.h"

using namespace re;

namespace re {

FileSystem& FileSystem::getInstance()
{
    static FileSystem instance;

    return instance;
}

Application::Application()
{
    this->view = new NativeWindow();
}

}