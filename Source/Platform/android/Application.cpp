#include "Application.h"

#include "NativeWindow.h"
#include "Platform/GameHub.h"
#include "FileSystemAndroid.h"
#include "Util/LogUtil.h"

#include <cmath>
#include <unistd.h>
#include <sys/time.h>

namespace re {

static Application* application;
static android_app* __state;
static AAssetManager* __assetManager;
static ASensorManager* __sensorManager;
static ASensorEventQueue* __sensorEventQueue;
static ASensorEvent __sensorEvent;
static const ASensor* __accelerometerSensor;
static const ASensor* __gyroscopeSensor;

static float __accelRawX;
static float __accelRawY;
static float __accelRawZ;
static float __gyroRawX;
static float __gyroRawY;
static float __gyroRawZ;
static int __orientationAngle = 90;

static long getCurrentMillSecond() {
    long lLastTime;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

int getRotation()
{
    jint rotation;

    // Get the android application's activity.
    ANativeActivity* activity = __state->activity;
    JavaVM* jvm = __state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR)
    {
        LOG_E("Failed to retrieve JVM environment when entering message pump.");
        return -1; 
    }
    RE_ASSERT(env);

    jclass clsContext = env->FindClass("android/content/Context");
    RE_ASSERT(clsContext != NULL);
    jmethodID getSystemService = env->GetMethodID(clsContext, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    RE_ASSERT(getSystemService != NULL);
    jfieldID WINDOW_SERVICE_ID = env->GetStaticFieldID(clsContext, "WINDOW_SERVICE", "Ljava/lang/String;");
    RE_ASSERT(WINDOW_SERVICE_ID != NULL);
    jstring WINDOW_SERVICE = (jstring) env->GetStaticObjectField(clsContext, WINDOW_SERVICE_ID);
    RE_ASSERT(WINDOW_SERVICE != NULL);
    jobject windowManager = env->CallObjectMethod(activity->clazz, getSystemService, WINDOW_SERVICE);
    RE_ASSERT(windowManager != NULL);
    jclass clsWindowManager = env->FindClass("android/view/WindowManager");
    RE_ASSERT(clsWindowManager != NULL);
    jmethodID getDefaultDisplay = env->GetMethodID(clsWindowManager, "getDefaultDisplay", "()Landroid/view/Display;");
    RE_ASSERT(getDefaultDisplay != NULL);
    jobject defaultDisplay = env->CallObjectMethod(windowManager, getDefaultDisplay);
    RE_ASSERT(defaultDisplay != NULL);
    jclass clsDisplay = env->FindClass("android/view/Display");
    RE_ASSERT(clsDisplay != NULL);
    jmethodID getRotation = env->GetMethodID(clsDisplay, "getRotation", "()I");
    RE_ASSERT(getRotation != NULL);
    rotation =  env->CallIntMethod(defaultDisplay, getRotation);

    return rotation;
}

static int32_t handle_input(android_app *app, AInputEvent *event)
{
    application->androidWin->handleInput(event);
    return 1;
}

static void handle_cmd(android_app *app, int32_t cmd)
{
    LOG_V("handle_cmd: %d", cmd);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                application->androidWin->initDisplay();

                application->__initialized = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            application->androidWin->destroySurface();
            application->__initialized = false;

            break;
        case APP_CMD_DESTROY:
            // TODO: exit the app
            application->androidWin->destroyDisplay();
            application->__initialized = false;
            break;
        case APP_CMD_RESUME:
            if (application->__initialized) {
                application->onEnterForeground();
            }
            application->__suspended = false;
            break;
        case APP_CMD_PAUSE:
            if (application->__initialized) {
                application->onExitForeground();
            }
            application->__suspended = true;
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (__accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(__sensorEventQueue, __accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(__sensorEventQueue, __accelerometerSensor, (1000L/60)*1000);
            }
            if (__gyroscopeSensor != NULL)
            {
                ASensorEventQueue_enableSensor(__sensorEventQueue, __gyroscopeSensor);
                // We'd like to get 60 events per second (in microseconds).
                ASensorEventQueue_setEventRate(__sensorEventQueue, __gyroscopeSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (__accelerometerSensor != NULL) 
            {
                ASensorEventQueue_disableSensor(__sensorEventQueue, __accelerometerSensor);
            }
            if (__gyroscopeSensor != NULL) 
            {
                ASensorEventQueue_disableSensor(__sensorEventQueue, __gyroscopeSensor);
            }

            break;
    }
}

static void handle_sensor()
{
    ASensorEventQueue_getEvents(__sensorEventQueue, &__sensorEvent, 1);
    if (__sensorEvent.type == ASENSOR_TYPE_ACCELEROMETER)
    {
        __accelRawX = __sensorEvent.acceleration.x;
        __accelRawY = __sensorEvent.acceleration.y;
        __accelRawZ = __sensorEvent.acceleration.z;
    }
    else if (__sensorEvent.type == ASENSOR_TYPE_GYROSCOPE)
    {
        __gyroRawX = __sensorEvent.vector.x;
        __gyroRawY = __sensorEvent.vector.y;
        __gyroRawZ = __sensorEvent.vector.z;
    }
}

Application::Application()
{
    application = this;

    this->view = new NativeWindow();
}

static bool __envInited = false;

void Application::run(android_app* state) {
    __state = state;

    this->initApp();

    this->androidWin = static_cast<NativeWindow*>(this->view);
    this->androidWin->initView(__state);

    FileSystemAndroid* fileSystem = static_cast<FileSystemAndroid*>(&FileSystem::getInstance());
    fileSystem->BindAssetManager(__assetManager);

    GameHub& game = GameHub::instance();
    game.init(this);

    long lastTime = getCurrentMillSecond();
    long curTime, dt, oldTime;
    long _animationInterval = 1 / 60.0 * 1000;

    while (true)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(!application->__suspended ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL) {
                // process main and input event, this will call onAppCmd and onInputEvent
                source->process(__state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER && __accelerometerSensor != NULL)
            {
                handle_sensor();
            }

            if (__state->destroyRequested != 0)
            {
                return;
            }
        }

        if (__initialized && !__envInited) {
            this->initEnvironment();

            __envInited = true;
        }

        // Idle time (no events left to process) is spent rendering.
        // We skip rendering when the app is paused.
        if (__initialized && !__suspended)
        {
            oldTime = lastTime;
            lastTime = getCurrentMillSecond();
            dt = lastTime - oldTime;

            // main loop and draw
            game.mainLoop(dt);

            curTime = getCurrentMillSecond();
            if (curTime - lastTime < _animationInterval)
            {
                usleep((_animationInterval - curTime + lastTime)*1000);
            }

            // Post the new frame to the display.
            // Note that there are a couple cases where eglSwapBuffers could fail
            // with an error code that requires a certain level of re-initialization:
            //
            // 1) EGL_BAD_NATIVE_WINDOW - Called when the surface we're currently using
            //    is invalidated. This would require us to destroy our EGL surface,
            //    close our OpenKODE window, and start again.
            //
            // 2) EGL_CONTEXT_LOST - Power management event that led to our EGL context
            //    being lost. Requires us to re-create and re-initalize our EGL context
            //    and all OpenGL ES state.
            //
            // For now, if we get these, we'll simply exit.
            int rc = application->androidWin->swapBuffers();
            if (rc != EGL_TRUE)
            {
                EGLint error = eglGetError();
                if (error == EGL_BAD_NATIVE_WINDOW)
                {
                    if (__state->window != NULL)
                    {
                        // recreate it
                        androidWin->destroySurface();
                        androidWin->initDisplay();
                    }
                    __initialized = true;
                }
                else
                {
                    LOG_E("eglSwapBuffers");
                    break;
                }
            }
        }

    }
}

void Application::initApp()
{
    RE_ASSERT(__state && __state->activity && __state->activity->vm);

    __initialized = false;
    __suspended = false;

    // Get the android application's activity.
    ANativeActivity* activity = __state->activity;
    JavaVM* jvm = __state->activity->vm;

    __assetManager = activity->assetManager;

    // Set the event call back functions.
    __state->onAppCmd = handle_cmd;
    __state->onInputEvent = handle_input;

    // Prepare to monitor accelerometer.
    __sensorManager = ASensorManager_getInstance();
    __accelerometerSensor = ASensorManager_getDefaultSensor(__sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    __gyroscopeSensor = ASensorManager_getDefaultSensor(__sensorManager, ASENSOR_TYPE_GYROSCOPE);
    __sensorEventQueue = ASensorManager_createEventQueue(__sensorManager, __state->looper, LOOPER_ID_USER, NULL, NULL);
}

void Application::getAccelerometerValues(float* pitch, float* roll)
{
    double tx, ty, tz;
    ASensorEvent event;
    
    // By default, android accelerometer values are oriented to the portrait mode.
    // flipping the x and y to get the desired landscape mode values.
    switch (androidWin->getOrientationAngle())
    {
    case 90:
        tx = -__accelRawY;
        ty = __accelRawX;
        break;
    case 180:
        tx = -__accelRawX;
        ty = -__accelRawY;
        break;
    case 270:
        tx = __accelRawY;
        ty = -__accelRawX;
        break;
    default:
        tx = __accelRawX;
        ty = __accelRawY;
        break;
    }
    tz = __accelRawZ;

    if (pitch != NULL)
    {
        RE_ASSERT(tx * tx + tz * tz);
        *pitch = -atan(ty / sqrt(tx * tx + tz * tz)) * 180.0f * M_1_PI;
    }
    if (roll != NULL)
    {
        RE_ASSERT(ty * ty + tz * tz);
        *roll = -atan(tx / sqrt(ty * ty + tz * tz)) * 180.0f * M_1_PI;
    }
}

void Application::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    if (accelX)
    {
        *accelX = __accelRawX;
    }

    if (accelY)
    {
        *accelY = __accelRawY;
    }

    if (accelZ)
    {
        *accelZ = __accelRawZ;
    }

    if (gyroX)
    {
        *gyroX = __gyroRawX;
    }

    if (gyroY)
    {
        *gyroY = __gyroRawY;
    }

    if (gyroZ)
    {
        *gyroZ = __gyroRawZ;
    }
}

} // namespace re
