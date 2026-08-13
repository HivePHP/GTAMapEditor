#include "Camera.h"
#include "Platform/Win32Window.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

Camera::Camera()
    : m_Position(0.0f, 5.0f, 15.0f), m_Yaw(-90.0f), m_Pitch(-20.0f),
    m_MoveSpeed(15.0f), m_MouseSensitivity(0.2f),
    m_LastMouseX(0), m_LastMouseY(0), m_FirstMouse(true)
{
    m_Up = Vector3(0.0f, 1.0f, 0.0f);
    UpdateVectors();
}

void Camera::UpdateVectors() {
    // Перевод углов в радианы
    float radYaw = m_Yaw * 3.14159f / 180.0f;
    float radPitch = m_Pitch * 3.14159f / 180.0f;

    m_Front.x = std::cos(radYaw) * std::cos(radPitch);
    m_Front.y = std::sin(radPitch);
    m_Front.z = std::sin(radYaw) * std::cos(radPitch);
    m_Front.Normalize();

    m_Right = m_Front.Cross(m_Up);
    m_Right.Normalize();
}

void Camera::Update(float deltaTime, const Win32Window& window) {
    float velocity = m_MoveSpeed * deltaTime;

    // Управление WASD
    if (window.IsKeyPressed('W')) m_Position += m_Front * velocity;
    if (window.IsKeyPressed('S')) m_Position -= m_Front * velocity;
    if (window.IsKeyPressed('A')) m_Position -= m_Right * velocity;
    if (window.IsKeyPressed('D')) m_Position += m_Right * velocity;

    // Вверх/вниз
    if (window.IsKeyPressed(VK_SPACE)) m_Position += m_Up * velocity;
    if (window.IsKeyPressed(VK_SHIFT)) m_Position -= m_Up * velocity;

    // Обзор мышью (при зажатой правой кнопке)
    int mouseX, mouseY;
    window.GetMousePosition(mouseX, mouseY);

    if (window.IsRightMouseButtonPressed()) {
        if (m_FirstMouse) {
            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;
            m_FirstMouse = false;
        }

        float xoffset = (float)(mouseX - m_LastMouseX) * m_MouseSensitivity;
        float yoffset = (float)(m_LastMouseY - mouseY) * m_MouseSensitivity; // Инвертированный Y

        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f) m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        UpdateVectors();
    }
    else {
        m_FirstMouse = true; // Сброс при отпускании ПКМ
    }
}

void Camera::ApplyView() const {
    Vector3 target = m_Position + m_Front;
    gluLookAt(
        m_Position.x, m_Position.y, m_Position.z,
        target.x, target.y, target.z,
        m_Up.x, m_Up.y, m_Up.z
    );
}