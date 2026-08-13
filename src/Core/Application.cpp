#include "Application.h"
#include "Platform/Win32Window.h"
#include "Platform/OpenGLContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"

#include "Logger.h"
#include "../GTA/GTAData.h"
#include "../GTA/GtaDatParser.h"
#include "../GTA/IMG/IMGArchive.h"
#include "../GTA/IDE/IDEParser.h"
#include "../GTA/IPL/IPLParser.h"

#include <filesystem>
#include <algorithm>
#include <GL/gl.h> // Подключаем OpenGL заголовки

namespace fs = std::filesystem;

Application::Application() : m_Running(true) {
    Logger::Init();
    Logger::Info("Starting GTA Map Editor");

    m_Window = std::make_unique<Win32Window>(L"GTA San Andreas Map Editor", 1280, 720);
    m_GLContext = std::make_unique<OpenGLContext>(m_Window->GetHWND());

    m_Renderer = std::make_unique<Renderer>();
    m_Renderer->Init();
    m_Camera = std::make_unique<Camera>();

    Logger::Info("OpenGL initialized");

    std::string gtaPath = "C:/Program Files (x86)/Grand Theft Auto San Andreas";
    Logger::Info("GTA path: " + gtaPath);

    GTAData gameData;
    if (GtaDatParser::Parse(gtaPath, gameData)) {
        Logger::Info("Successfully parsed gta.dat");

        std::string gta3Path = gtaPath + "/models/gta3.img";
        IMGArchive gta3Archive;
        gta3Archive.Open(gta3Path);

        std::vector<std::string> allIdePaths;
        for (const std::string& p : gameData.m_IdeFiles) allIdePaths.push_back(gtaPath + "/" + p);

        std::vector<std::string> allIplPaths;
        for (const std::string& p : gameData.m_IplFiles) allIplPaths.push_back(gtaPath + "/" + p);

        std::string vcMapPath = gtaPath + "/vc_map";
        if (fs::exists(vcMapPath) && fs::is_directory(vcMapPath)) {
            Logger::Info("Found custom map folder: " + vcMapPath + ". Scanning...");
            for (const auto& entry : fs::recursive_directory_iterator(vcMapPath)) {
                if (entry.is_regular_file()) {
                    std::string ext = entry.path().extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                    if (ext == ".ide") {
                        allIdePaths.push_back(entry.path().string());
                    }
                    else if (ext == ".ipl") {
                        allIplPaths.push_back(entry.path().string());
                    }
                }
            }
        }

        IDEParser ideParser;
        Logger::Info("Loading IDE files...");
        for (const std::string& fullPath : allIdePaths) {
            ideParser.Load(fullPath);
        }
        Logger::Info("IDE objects loaded: " + std::to_string(ideParser.GetObjectCount()));

        IPLParser iplParser;
        Logger::Info("Loading IPL files...");
        for (const std::string& fullPath : allIplPaths) {
            iplParser.Load(fullPath);
        }
        Logger::Info("IPL placements loaded: " + std::to_string(iplParser.GetPlacementCount()));

        // Передаем данные в UI
        m_Window->SetIDEData(ideParser.GetObjectsList());
        m_Window->SetIPLData(iplParser.GetPlacements());
        Logger::Info("Data successfully passed to UI");

    }
    else {
        Logger::Error("Failed to parse gta.dat. Check your GTA path.");
    }
}

Application::~Application() {
}

void Application::Run() {
    while (m_Running) {
        if (!m_Window->ProcessMessages()) {
            m_Running = false;
            break;
        }

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ::SwapBuffers(wglGetCurrentDC());
    }
}