#include "Camera.h"
#include "../Platform/Win32Window.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <windows.h>

Camera::Camera() {}
Camera::~Camera() {}

void Camera::Update(Win32Window* window, float deltaTime) {
    if (!window) return;

    float velocity = m_Speed * deltaTime;
    if (window->IsKeyPressed('W')) {
        m_PosX += velocity * sin(m_Yaw * 3.14159f / 180.0f);
        m_PosZ -= velocity * cos(m_Yaw * 3.14159f / 180.0f);
    }
    if (window->IsKeyPressed('S')) {
        m_PosX -= velocity * sin(m_Yaw * 3.14159f / 180.0f);
        m_PosZ += velocity * cos(m_Yaw * 3.14159f / 180.0f);
    }
    if (window->IsKeyPressed('A')) {
        m_PosX -= velocity * cos(m_Yaw * 3.14159f / 180.0f);
        m_PosZ -= velocity * sin(m_Yaw * 3.14159f / 180.0f);
    }
    if (window->IsKeyPressed('D')) {
        m_PosX += velocity * cos(m_Yaw * 3.14159f / 180.0f);
        m_PosZ += velocity * sin(m_Yaw * 3.14159f / 180.0f);
    }
    if (window->IsKeyPressed(VK_SPACE)) {
        m_PosY += velocity;
    }
    if (window->IsKeyPressed(VK_CONTROL)) {
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