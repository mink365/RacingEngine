#include "NativeWindow.h"

#include "Util/LogUtil.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "UI/TouchEvent.h"
#include "Message/MessageManager.h"
#include "Message/MessageConstant.h"

namespace re {

NativeWindow::NativeWindow()
{
}

void NativeWindow::initView(android_app *app)
{
    this->__app = app;
}

void NativeWindow::dispatchTouchEvent(TouchEventType type, float x, float y)
{
    auto event = std::make_shared<TouchEvent>();

    const Rect& rect = this->viewRect;

    event->setInfo(type, x - rect.origin.x,
                   rect.size.height - (y - rect.origin.y));

    MessageManager::getInstance()->sendNoKeyMessage(MessageConstant::MessageType::TOUCHSCREEN_MESSAGE, event);
}

int32_t NativeWindow::handleInput(AInputEvent *event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event);

        size_t pointerIndex;
        size_t pointerId;
        size_t pointerCount;
        int x;
        int y;

        switch ( action & AMOTION_EVENT_ACTION_MASK) {
        case AMOTION_EVENT_ACTION_DOWN:
        {
            pointerId = AMotionEvent_getPointerId(event, 0);
            x = AMotionEvent_getX(event, 0);
            y = AMotionEvent_getY(event, 0);

            dispatchTouchEvent(TouchEventType::DOWN, x, y);

            break;
        }
        case AMOTION_EVENT_ACTION_MOVE:
        {
            // ACTION_MOVE events are batched, unlike the other events.
            pointerCount = AMotionEvent_getPointerCount(event);
            for (size_t i = 0; i < pointerCount; ++i) {
                pointerId = AMotionEvent_getPointerId(event, i);
                x = AMotionEvent_getX(event, i);
                y = AMotionEvent_getY(event, i);

                dispatchTouchEvent(TouchEventType::MOVE, x, y);
            }

            break;
        }
        case AMOTION_EVENT_ACTION_UP:
        {
            pointerId = AMotionEvent_getPointerId(event, 0);
            x = AMotionEvent_getX(event, 0);
            y = AMotionEvent_getY(event, 0);

            dispatchTouchEvent(TouchEventType::UP, x, y);

            break;
        }
        case AMOTION_EVENT_ACTION_CANCEL:
        {
            pointerId = AMotionEvent_getPointerId(event, 0);
            x = AMotionEvent_getX(event, 0);
            y = AMotionEvent_getY(event, 0);

            dispatchTouchEvent(TouchEventType::CANCEL, x, y);

            break;
        }

        }

        return 1;
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {

        return 1;
    }

    return 1;
}

int NativeWindow::swapBuffers()
{
    int rc = eglSwapBuffers(display, surface);

    return rc;
}

void NativeWindow::setFrameSize(float width, float height) {

}

Size NativeWindow::getFrameSize() const {
    return viewRect.size;
}

int NativeWindow::initDisplay()
{
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(__app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, __app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOG_D("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    this->display = display;
    this->context = context;
    this->surface = surface;

    this->viewRect.set(0, 0, w, h);

    return 0;
}

void NativeWindow::termDisplay()
{
    if (this->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (this->context != EGL_NO_CONTEXT) {
            eglDestroyContext(this->display, this->context);
        }
        if (this->surface != EGL_NO_SURFACE) {
            eglDestroySurface(this->display, this->surface);
        }
        eglTerminate(this->display);
    }
    this->display = EGL_NO_DISPLAY;
    this->context = EGL_NO_CONTEXT;
    this->surface = EGL_NO_SURFACE;
}

} // namespace re
