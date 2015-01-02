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

extern int getRotation();

NativeWindow::NativeWindow()
{
    __app = 0;
    display = 0;
    context = 0;
    surface = 0;

    orientationAngle = 0;
    vsync = true;
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

    MessageManager::getInstance().sendNoKeyMessage(MessageConstant::MessageType::TOUCHSCREEN_MESSAGE, event);
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
    if (!display || !surface) {
        return 1;
    }

    int rc = eglSwapBuffers(display, surface);

    return rc;
}

void NativeWindow::setFrameSize(float width, float height) {

}

Size NativeWindow::getFrameSize() const {
    return viewRect.size;
}

void NativeWindow::setVsync(bool enable)
{
    eglSwapInterval(this->display, enable ? 1 : 0);

    vsync = enable;
}

bool NativeWindow::isVsync() const
{
    return vsync;
}

int NativeWindow::getOrientationAngle()
{
    return orientationAngle;
}

static EGLenum checkErrorEGL(const char* msg)
{
//    GP_ASSERT(msg);
    static const char* errmsg[] =
    {
        "EGL function succeeded",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "EGL cannot access a requested resource",
        "EGL failed to allocate resources for the requested operation",
        "EGL fail to access an unrecognized attribute or attribute value was passed in an attribute list",
        "EGLConfig argument does not name a valid EGLConfig",
        "EGLContext argument does not name a valid EGLContext",
        "EGL current surface of the calling thread is no longer valid",
        "EGLDisplay argument does not name a valid EGLDisplay",
        "EGL arguments are inconsistent",
        "EGLNativePixmapType argument does not refer to a valid native pixmap",
        "EGLNativeWindowType argument does not refer to a valid native window",
        "EGL one or more argument values are invalid",
        "EGLSurface argument does not name a valid surface configured for rendering",
        "EGL power management event has occurred",
    };
    EGLenum error = eglGetError();
    LOG_E("%s: %s.", msg, errmsg[error - EGL_SUCCESS]);
    return error;
}

int NativeWindow::initDisplay()
{
    // initialize OpenGL ES and EGL
    // on phone rotate or app create

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    int samples = 4;
    EGLint eglConfigAttrs[] =
    {
        EGL_SAMPLE_BUFFERS,     samples > 0 ? 1 : 0,
        EGL_SAMPLES,            samples,
        EGL_DEPTH_SIZE,         24,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_STENCIL_SIZE,       8,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    const EGLint eglContextAttrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION,    2,
        EGL_NONE
    };

    const EGLint eglSurfaceAttrs[] =
    {
        EGL_RENDER_BUFFER,    EGL_BACK_BUFFER,
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
    eglChooseConfig(display, eglConfigAttrs, &config, 1, &numConfigs);

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, eglContextAttrs);

    if (context == EGL_NO_CONTEXT)
    {
        checkErrorEGL("eglCreateContext");

        return -1;
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(__app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, __app->window, eglSurfaceAttrs);
    if (surface == EGL_NO_SURFACE)
    {
        checkErrorEGL("eglCreateWindowSurface");
        return -1;
    }

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOG_E("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    eglSwapInterval(display, vsync ? 1 : 0);

    LOG_D("EGL display: %p, context: %p, App: %p, WH: %d %d", display, context, __app, w, h);

    this->display = display;
    this->context = context;
    this->surface = surface;

    this->viewRect.set(0, 0, w, h);

    this->orientationAngle = getRotation() * 90;

    return 0;
}

void NativeWindow::destroySurface()
{
    if (this->display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    if (this->surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(this->display, this->surface);
        this->surface = EGL_NO_SURFACE;
    }
}

void NativeWindow::destroyDisplay()
{
    destroySurface();

    if (this->context != EGL_NO_CONTEXT)
    {
        eglDestroyContext(this->display, this->context);
        this->context = EGL_NO_CONTEXT;
    }

    if (this->display != EGL_NO_DISPLAY)
    {
        eglTerminate(this->display);
        this->display = EGL_NO_DISPLAY;
    }
}

} // namespace re
