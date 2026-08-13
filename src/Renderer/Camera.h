#pragma once
#include "Math/Vector3.h"

class Win32Window; // Forward declaration

class Camera {
public:
    Camera();

    void Update(float deltaTime, const Win32Window& window);
    void ApplyView() const;

    Vector3 GetPosition() const { return m_Position; }

private:
    Vector3 m_Position;
    Vector3 m_Front;
    Vector3 m_Up;
    Vector3 m_Right;

    float m_Yaw;
    float m_Pitch;
    float m_MoveSpeed;
    float m_MouseSensitivity;

    int m_LastMouseX;
    int m_LastMouseY;
    bool m_FirstMouse;

    void UpdateVectors();
};