#pragma once

class Win32Window;

class Camera {
public:
    Camera();
    ~Camera();

    void Update(Win32Window* window, float deltaTime);
    void ApplyView() const;

private:
    float m_PosX = 0.0f;
    float m_PosY = 50.0f;
    float m_PosZ = 100.0f;
    float m_Pitch = -20.0f;
    float m_Yaw = 0.0f;
    float m_Speed = 50.0f;
    float m_Sensitivity = 0.1f;

    bool m_FirstMouse = true;
    float m_LastMouseX = 0.0f;
    float m_LastMouseY = 0.0f;
};