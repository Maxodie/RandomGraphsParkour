#include "Window/Window.h"
#include "Log/Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void RGP_WindowRrrorCallback(int32_t error, const char* desc)
{
    RGP_LOG_ERROR("GLFW error : %s", desc);
}

static void RGP_WindowCloseCallback(GLFWwindow* native)
{
    RGP_Window_t* window = (RGP_Window_t*)glfwGetWindowUserPointer(native);

    if(window->EventCallback)
    {
        window->EventCallback(window->ctx, RPG_WINDOW_EVENT_TYPE_CLOSE);
    }
    else
    {
        RGP_LOG_WARNING("Window event callback is NULL");
    }
}

Bool RGP_WindowInitializeAPI()
{
    Bool result = glfwInit() == GLFW_TRUE;
    glfwSetErrorCallback(RGP_WindowRrrorCallback);
    return result;
}

Bool RGP_WindowCreate(
        RGP_Window_t* outWindow, RGP_Application_t* application,
        int32_t width, int32_t height, const char* name,
        void (*EventCallback)(RGP_Application_t*, RPG_WindowEventType)
        )
{
    RGP_ASSERT(outWindow != NULL, "outWindow should not be NULL");
    RGP_ASSERT(application != NULL, "application should not be NULL");

    memset(outWindow, 0, sizeof(RGP_Window_t));

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    outWindow->native = glfwCreateWindow(width, height, name, NULL, NULL);
    if(!outWindow->native)
    {
        RGP_ASSERT(0, "outWindow should not be NULL");
        return 0;
    }
    outWindow->ctx = application;
    outWindow->width = width;
    outWindow->height = height;
    outWindow->name = name;
    outWindow->EventCallback = EventCallback;

    glfwSetWindowUserPointer(outWindow->native, (void*)outWindow);

    glfwSetWindowCloseCallback(outWindow->native, RGP_WindowCloseCallback);


    glfwMakeContextCurrent(outWindow->native);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    const GLubyte * version = glGetString(GL_VERSION);
    RGP_LOG_DEBUG("OpenGL Version : %s", version);

    RGP_LOG_SUCCESS("Window created");
    return 1;
}

void RGP_WindowDestroy(RGP_Window_t* window)
{
    RGP_ASSERT(window != NULL, "window should not be NULL");

    glfwSetCursor(window->native, window->native);
	glfwDestroyWindow(window->native);
	glfwTerminate();

    RGP_LOG_SUCCESS("Window destroyed");
}

void RGP_WindowPoll()
{
	glfwPollEvents();
}

void RGP_WindowSwapBuffers(RGP_Window_t* window)
{
	glfwSwapBuffers(window->native);
}
