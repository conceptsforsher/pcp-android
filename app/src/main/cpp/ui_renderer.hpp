#pragma once

#include <GLES3/gl3.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

// ── UI Renderer for PCP ──
// Pure C++ OpenGL ES 3.0 renderer (no Qt dependency)

class UIRenderer {
public:
    UIRenderer(int32_t screenWidth, int32_t screenHeight);
    ~UIRenderer();

    void update(double deltaTime);
    void render();

    // Screen info
    int32_t getWidth() const { return m_width; }
    int32_t getHeight() const { return m_height; }
    void resize(int32_t w, int32_t h);

    // Rendering state
    void setBackgroundColor(float r, float g, float b, float a);

private:
    int32_t m_width, m_height;

    // Shader programs
    GLuint m_basicProgram = 0;
    GLuint m_textProgram = 0;
    GLuint m_textureProgram = 0;

    // Common uniforms
    GLint m_mvpLoc = -1;
    GLint m_colorLoc = -1;
    GLint m_texLoc = -1;

    // Background
    float m_bgR = 0.059f, m_bgG = 0.071f, m_bgB = 0.106f, m_bgA = 1.0f;

    // Vertex buffer for rectangles
    GLuint m_rectVAO = 0, m_rectVBO = 0, m_rectEBO = 0;

    void initShaders();
    void initGeometry();
    GLuint loadShader(GLenum type, const char* source);
    GLuint createProgram(const char* vsSource, const char* fsSource);

    // Rect drawing
    void drawRect(float x, float y, float w, float h, float r, float g, float b, float a);
    void drawRoundedRect(float x, float y, float w, float h, float radius,
                         float r, float g, float b, float a);
};
