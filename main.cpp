#include <iostream>
#include <cstring>
#include <cassert>
#include <stdio.h>

#define X11 

#ifdef X11
#include  <X11/Xlib.h>
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

static EGLNativeDisplayType display;
static EGLNativeWindowType window;

static const int kWidth = 720;
static const int kHeight = 576;

#ifdef X11
static void createBasicObjects()
{
    display = XOpenDisplay(0);
    assert(display != 0);

    Window root = DefaultRootWindow(display);
    XSetWindowAttributes swa;
    memset(&swa, 0, sizeof(swa));
    Window win  = XCreateWindow(display, root, 0, 0, kWidth, kHeight, 0, CopyFromParent,
                                InputOutput, CopyFromParent, CWEventMask, &swa);
    XMapWindow(display, win);
    XStoreName(display, win, "EGLFS");
    window = win;
}
#else
static void createBasicObjects()
{
    assert(!"not reachable");
}
#endif

int main(int argc, char* argv[])
{
    createBasicObjects();

    EGLBoolean result;

    static const EGLint attribute_list[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLint major, minor;
    if (!eglBindAPI(EGL_OPENGL_ES_API)) {
        printf("eglBindAPI Error\n");
        return 1;
    }

    EGLDisplay eglDisplay = eglGetDisplay( display ? display : EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        printf("eglGetDisplay Error %p\n",eglDisplay);
        return 2;
    }

    if (!eglInitialize(eglDisplay, &major, &minor)) {
        printf("eglInitialize Error\n");
        return 3;
    }

    EGLConfig config;
    EGLint num_config;    
    /* Get an appropriate EGL frame buffer config. */
    result = eglChooseConfig(eglDisplay, attribute_list, &config, 1, &num_config);
    assert(result != EGL_FALSE);

    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);

    static const EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLContext context = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, context_attributes);
    assert(context != EGL_NO_CONTEXT);

    EGLSurface surface = eglCreateWindowSurface(eglDisplay, config, window, NULL);
    assert(surface != EGL_NO_SURFACE);

    result = eglMakeCurrent(eglDisplay, surface, surface, context);
    assert(result != EGL_FALSE);
    assert(glGetError() == 0);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return 0;
}