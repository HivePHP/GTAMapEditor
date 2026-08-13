#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Renderer/Mesh.h"
#include "../GTA/IMG/IMGArchive.h"

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
    bool m_Running;
    std::unique_ptr<Win32Window> m_Window;
    std::unique_ptr<OpenGLContext> m_GLContext;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Camera> m_Camera;

    // Кэш и архивы для загрузки реальных 3D-моделей
    IMGArchive m_Gta3Archive;
    std::unordered_map<int, std::string> m_ModelIdToName;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> m_ModelCache;

    std::shared_ptr<Mesh> GetOrLoadModel(int modelId);
};