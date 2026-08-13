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
#include <chrono>
#include <GL/gl.h>
#include <GL/glu.h>

namespace fs = std::filesystem;

static void DrawGrid() {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    int size = 100;
    int step = 10;

    for (int i = -size; i <= size; i += step) {
        if (i == 0) {
            glColor3f(0.5f, 0.5f, 0.5f);
        }
        else {
            glColor3f(0.25f, 0.25f, 0.25f);
        }

        glVertex3f((float)-size, 0.0f, (float)i);
        glVertex3f((float)size, 0.0f, (float)i);

        glVertex3f((float)i, 0.0f, (float)-size);
        glVertex3f((float)i, 0.0f, (float)size);
    }

    glEnd();
}

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
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (m_Running) {
        if (!m_Window->ProcessMessages()) {
            m_Running = false;
            break;
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Передаем deltaTime в камеру
        if (m_Camera) {
            m_Camera->Update(m_Window.get(), deltaTime);
        }

        glViewport(0, 0, 1280, 720);
        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 10000.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if (m_Camera) {
            m_Camera->Update(m_Window.get(), deltaTime);
        }

        DrawGrid();

        // Корректная смена буферов для Win32/OpenGL контекста
        ::SwapBuffers(wglGetCurrentDC());
    }
}