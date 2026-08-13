#pragma once
#include <windows.h>

class OpenGLContext {
public:
    OpenGLContext(HWND hwnd);
    ~OpenGLContext();

    void MakeCurrent();
    void Swap();

private:
    HWND m_Hwnd;
    HDC m_Hdc;
    HGLRC m_Hglrc;
};