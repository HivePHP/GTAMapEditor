#include "Camera.h"
#include "../Platform/Win32Window.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <windows.h>

Camera::Camera() {
    m_Speed = 150.0f;
}

Camera::~Camera() {}

void Camera::Update(Win32Window* window, float deltaTime) {
    if (!window) return;

    float velocity = m_Speed * deltaTime;

    bool wPressed = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('w') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    bool sPressed = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    bool aPressed = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    bool dPressed = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);

    float radPitch = m_Pitch * 3.14159f / 180.0f;
    float radYaw = m_Yaw * 3.14159f / 180.0f;

    // Полный вектор направления взгляда (включая высоту Y)
    float frontX = sin(radYaw) * cos(radPitch);
    float frontY = sin(radPitch);
    float frontZ = -cos(radYaw) * cos(radPitch);

    // Вектор стрейфа влево/вправо (параллельно плоскости земли)
    float rightX = cos(radYaw);
    float rightZ = sin(radYaw);

    if (wPressed) {
        m_PosX += frontX * velocity;
        m_PosY += frontY * velocity;
        m_PosZ += frontZ * velocity;
    }
    if (sPressed) {
        m_PosX -= frontX * velocity;
        m_PosY -= frontY * velocity;
        m_PosZ -= frontZ * velocity;
    }
    if (aPressed) {
        m_PosX -= rightX * velocity;
        m_PosZ -= rightZ * velocity;
    }
    if (dPressed) {
        m_PosX += rightX * velocity;
        m_PosZ += rightZ * velocity;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        m_PosY += velocity;
    }
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        m_PosY -= velocity;
    }

    if (window->IsRightMouseButtonPressed()) {
        int mouseX = 0, mouseY = 0;
        window->GetMousePosition(mouseX, mouseY);

        if (m_FirstMouse) {
            m_LastMouseX = static_cast<float>(mouseX);
            m_LastMouseY = static_cast<float>(mouseY);
            m_FirstMouse = false;
        }

        float xoffset = static_cast<float>(mouseX) - m_LastMouseX;
        float yoffset = m_LastMouseY - static_cast<float>(mouseY);

        m_LastMouseX = static_cast<float>(mouseX);
        m_LastMouseY = static_cast<float>(mouseY);

        xoffset *= m_Sensitivity;
        yoffset *= m_Sensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;
    }
    else {
        m_FirstMouse = true;
    }
}

void Camera::ApplyView() const {
    float radPitch = m_Pitch * 3.14159f / 180.0f;
    float radYaw = m_Yaw * 3.14159f / 180.0f;

    float dirX = sin(radYaw) * cos(radPitch);
    float dirY = sin(radPitch);
    float dirZ = -cos(radYaw) * cos(radPitch);

    gluLookAt(
        m_PosX, m_PosY, m_PosZ,
        m_PosX + dirX, m_PosY + dirY, m_PosZ + dirZ,
        0.0f, 1.0f, 0.0f
    );
}