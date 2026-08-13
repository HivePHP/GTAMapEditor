#include "OpenGLContext.h"
#include <stdexcept>

OpenGLContext::OpenGLContext(HWND hwnd) : m_Hwnd(hwnd) {
    m_Hdc = GetDC(m_Hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_Hdc, &pfd);
    if (pixelFormat == 0) {
        throw std::runtime_error("Failed to choose pixel format");
    }

    if (!SetPixelFormat(m_Hdc, pixelFormat, &pfd)) {
        throw std::runtime_error("Failed to set pixel format");
    }

    m_Hglrc = wglCreateContext(m_Hdc);
    if (!m_Hglrc) {
        throw std::runtime_error("Failed to create WGL context");
    }

    MakeCurrent();
}

OpenGLContext::~OpenGLContext() {
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(m_Hglrc);
    ReleaseDC(m_Hwnd, m_Hdc);
}

void OpenGLContext::MakeCurrent() {
    wglMakeCurrent(m_Hdc, m_Hglrc);
}

void OpenGLContext::Swap() {
    SwapBuffers(m_Hdc);
}