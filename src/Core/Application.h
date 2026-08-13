#pragma once
#include <memory>

class Win32Window;
class OpenGLContext;
class Renderer;
class Camera;

class Application {
public:
    Application();
    ~Application();

    void Run();

private:
    std::unique_ptr<Win32Window> m_Window;
    std::unique_ptr<OpenGLContext> m_GLContext;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Camera> m_Camera;

    bool m_Running;
};