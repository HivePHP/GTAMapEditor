#pragma once

class Camera; // Forward declaration

class Renderer {
public:
    void Init();
    void Resize(int width, int height);
    void BeginFrame();
    void RenderScene(const Camera& camera);

private:
    void DrawGrid();
    void DrawAxes();
};