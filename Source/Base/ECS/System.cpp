#include "System.h"
#include "Event.h"

namespace re {

System::System()
{

}

SystemManager::SystemManager()
    : eventManager_(EventManager::instance())
{
}

void SystemManager::configure()
{
    for (auto& pair : systems_) {
        pair.second->configure(eventManager_);
    }
}

} // namespace re

