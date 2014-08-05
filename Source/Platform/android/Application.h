#ifndef RE_APPLICATION_H
#define RE_APPLICATION_H

#include "Platform/ApplicationProtocol.h"
#include "NativeWindow.h"
#include <android_native_app_glue.h>
#include <android/sensor.h>

namespace re {

class Application : public ApplicationProtocol
{
public:
    Application();

    void run(android_app *state);

protected:
    void initApp();

public:
    NativeWindow* androidWin;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    bool __initialized;
    bool __suspended;
};

} // namespace re

#endif // RE_APPLICATION_H
