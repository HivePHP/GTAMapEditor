#include "Renderer.h"
#include "Camera.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // Не рендерим невидимые объекты
    glClearColor(0.2f, 0.25f, 0.3f, 1.0f); // Темно-синий фон редактора
}

void Renderer::Resize(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Угол обзора 45 градусов, near = 0.1, far = 10000.0 (карты GTA большие)
    gluPerspective(45.0, (double)width / (double)height, 0.1, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void Renderer::RenderScene(const Camera& camera) {
    camera.ApplyView();

    DrawGrid();
    DrawAxes();
}

void Renderer::DrawGrid() {
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f); // Серый цвет
    int gridSize = 50;
    for (int i = -gridSize; i <= gridSize; i += 2) {
        // Линии вдоль оси Z
        glVertex3f((float)i, 0.0f, (float)-gridSize);
        glVertex3f((float)i, 0.0f, (float)gridSize);
        // Линии вдоль оси X
        glVertex3f((float)-gridSize, 0.0f, (float)i);
        glVertex3f((float)gridSize, 0.0f, (float)i);
    }
    glEnd();
}

void Renderer::DrawAxes() {
    glDisable(GL_DEPTH_TEST); // Оси рисуем поверх всего
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // X - Красный
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);

    // Y - Зеленый
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);

    // Z - Синий
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 5.0f);

    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_DEPTH_TEST);
}