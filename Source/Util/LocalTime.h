#ifndef __LocalTime__
#define __LocalTime__

#include "Base/Singleton.h"

class LocalTime : public re::Singleton<LocalTime>
{
    friend class Singleton;

public:
    long getCurrentTime();
private:
    LocalTime();
};

#endif
