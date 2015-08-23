#ifndef __LocalTime__
#define __LocalTime__

#include "Base/Singleton.h"

class LocalTime : public re::Singleton<LocalTime>
{
    friend class Singleton;

public:
    /**
     * @brief getCurrentTime
     * Seconds from 1970
     * @return
     */
    long getCurrentTime();
private:
    LocalTime();
};

#endif
