#include "NativeWindow.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Math/Rect.h"
#include <string>
#include "UI/TouchEvent.h"
#include "Message/MessageManager.h"
#include "Message/MessageConstant.h"

#include <stdlib.h>

namespace re {

using namespace ui;

class GLFWEventHandler
{
private:
    static bool _captured;
    static float _mouseX, _mouseY;

public:
    static NativeWindow* nativeView;

public:
    static void onGLFWError(int errorID, const char* errorDesc)
    {

    }

    static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
    {
        if(GLFW_MOUSE_BUTTON_LEFT == button)
        {
            if(GLFW_PRESS == action)
            {
                _captured = true;
                if (getWindowRect().containsPoint(Vec2(_mouseX,_mouseY)))
                {
                    dispatchTouchEvent(TouchEventType::DOWN, _mouseX, _mouseY);
                }
            }
            else if(GLFW_RELEASE == action)
            {
                if (_captured)
                {
                    _captured = false;
                    dispatchTouchEvent(TouchEventType::UP, _mouseX, _mouseY);
                }
            }
        }
    }

    static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
    {
        _mouseX = (float)x;
        _mouseY = (float)y;

        if (_captured)
        {
            dispatchTouchEvent(TouchEventType::MOVE, _mouseX, _mouseY);
        }

    }

    static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
    {

    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {

    }

    static void onGLFWCharCallback(GLFWwindow* window, unsigned int character)
    {

    }

    static void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
    {

    }

    static void onGLFWframebuffersize(GLFWwindow* window, int w, int h)
    {
        nativeView->setFrameSize(w, h);
    }

    static void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
    {

    }

private:
    static Rect getWindowRect() {
        return Rect(Vec2(0, 0),nativeView->windowSize);
    }

    static void dispatchTouchEvent(TouchEventType type, float x, float y) {
        auto event = Create<TouchEvent>();

        const Rect rect = getWindowRect();

        float x_ = (x - rect.origin.x) / rect.size.width * nativeView->framebufferSize.width;
        float y_ = (rect.size.height - (y - rect.origin.y)) / rect.size.height * nativeView->framebufferSize.height;
        event->setInfo(type, x_, y_);

        MessageManager::instance().sendNoKeyMessage(MessageConstant::MessageType::TOUCHSCREEN_MESSAGE, event);
    }
};

bool GLFWEventHandler::_captured = false;
float GLFWEventHandler::_mouseX = 0;
float GLFWEventHandler::_mouseY = 0;
NativeWindow* GLFWEventHandler::nativeView = NULL;

NativeWindow::NativeWindow()
{
    this->windowSize = Size(800, 600);
}

bool NativeWindow::initView()
{
    std::string _viewName = "Hello";
    GLFWmonitor* _monitor = nullptr;

    glfwSetErrorCallback(GLFWEventHandler::onGLFWError);

    if ( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW!\n" );
        return -1;
    }

//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(windowSize.width,
                                   windowSize.height,
                                   _viewName.c_str(),
                                   _monitor,
                                   nullptr);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLFWEventHandler::nativeView = this;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    this->setFrameSize(width, height);

    // check OpenGL version at first
    const GLubyte* glVendor = glGetString(GL_VENDOR);
    const GLubyte* glRenderer = glGetString(GL_RENDERER);
    const GLubyte* glVersion = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GL Vendor:    %s\n"
           "GL Renderer:  %s\n"
           "GL version:   %s\n"
           "GLSL version: %s\n", glVendor, glRenderer, glVersion, glslVersion);

    if ( atof((const char*)glVersion) < 1.5 )
    {
        char strComplain[256] = {0};
        sprintf(strComplain,
                "OpenGL 1.5 or higher is required (your version is %s). Please upgrade the driver of your video card.",
                glVersion);
//        MessageBox(strComplain, "OpenGL version too old");
        return false;
    }

    this->bindEventHandler();

    initGlew();

    // Enable point size by default.
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    return true;
}

void NativeWindow::bindEventHandler()
{
    glfwSetMouseButtonCallback(window, GLFWEventHandler::onGLFWMouseCallBack);
    glfwSetCursorPosCallback(window, GLFWEventHandler::onGLFWMouseMoveCallBack);
    glfwSetScrollCallback(window, GLFWEventHandler::onGLFWMouseScrollCallback);
    glfwSetCharCallback(window, GLFWEventHandler::onGLFWCharCallback);
    glfwSetKeyCallback(window, GLFWEventHandler::onGLFWKeyCallback);
    glfwSetWindowPosCallback(window, GLFWEventHandler::onGLFWWindowPosCallback);
    glfwSetFramebufferSizeCallback(window, GLFWEventHandler::onGLFWframebuffersize);
    glfwSetWindowSizeCallback(window, GLFWEventHandler::onGLFWWindowSizeFunCallback);
}

void NativeWindow::setFrameSize(float width, float height)
{
    this->framebufferSize.set(width, height);
}

Size NativeWindow::getFrameSize() const
{
    return this->framebufferSize;
}

bool NativeWindow::shouldClose()
{
    if (window) {
        return glfwWindowShouldClose(window);
    }

    return true;
}

void NativeWindow::swapBuffers()
{
    if (window) {
        glfwSwapBuffers(window);
    }
}

void NativeWindow::pollEvents()
{
    if (window) {
        glfwPollEvents();
    }
}

void log(const std::string& v) {

}

bool NativeWindow::initGlew()
{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
//        MessageBox((char *)glewGetErrorString(GlewInitResult), "OpenGL error");
        return false;
    }

    // TODO:
//    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
//    {
//        log("Ready for GLSL");
//    }
//    else
//    {
//        log("Not totally ready :(");
//    }

    if (glewIsSupported("GL_VERSION_2_0"))
    {
        log("Ready for OpenGL 2.0");
    }
    else
    {
        log("OpenGL 2.0 not supported");
    }

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    if(glew_dynamic_binding() == false)
//    {
//        MessageBox("No OpenGL framebuffer support. Please upgrade the driver of your video card.", "OpenGL error");
//        return false;
//    }
//#endif

//#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

    return true;
}

} // namespace re
