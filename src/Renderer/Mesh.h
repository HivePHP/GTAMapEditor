#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <cstddef>

#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

typedef ptrdiff_t GLsizeiptr;

typedef GLuint(APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);

inline PFNGLGENVERTEXARRAYSPROC p_glGenVertexArrays = nullptr;
inline PFNGLBINDVERTEXARRAYPROC p_glBindVertexArray = nullptr;
inline PFNGLDELETEVERTEXARRAYSPROC p_glDeleteVertexArrays = nullptr;
inline PFNGLGENBUFFERSPROC p_glGenBuffers = nullptr;
inline PFNGLBINDBUFFERPROC p_glBindBuffer = nullptr;
inline PFNGLBUFFERDATAPROC p_glBufferData = nullptr;
inline PFNGLDELETEBUFFERSPROC p_glDeleteBuffers = nullptr;
inline PFNGLVERTEXATTRIBPOINTERPROC p_glVertexAttribPointer = nullptr;
inline PFNGLENABLEVERTEXATTRIBARRAYPROC p_glEnableVertexAttribArray = nullptr;

#define glGenVertexArrays p_glGenVertexArrays
#define glBindVertexArray p_glBindVertexArray
#define glDeleteVertexArrays p_glDeleteVertexArrays
#define glGenBuffers p_glGenBuffers
#define glBindBuffer p_glBindBuffer
#define glBufferData p_glBufferData
#define glDeleteBuffers p_glDeleteBuffers
#define glVertexAttribPointer p_glVertexAttribPointer
#define glEnableVertexAttribArray p_glEnableVertexAttribArray

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif

inline void InitOpenGLExtensions() {
    static bool loaded = false;
    if (loaded) return;
    p_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    p_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    p_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
    p_glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    p_glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    p_glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    p_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    p_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    p_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    loaded = true;
}

struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    void UploadToGPU() {
        InitOpenGLExtensions();
        if (vertices.empty() || indices.empty()) return;

        if (glGenVertexArrays) glGenVertexArrays(1, &vao);
        if (glGenBuffers) glGenBuffers(1, &vbo);
        if (glGenBuffers) glGenBuffers(1, &ebo);

        if (glBindVertexArray) glBindVertexArray(vao);

        if (glBindBuffer) glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if (glBufferData) glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        if (glBindBuffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        if (glBufferData) glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        if (glVertexAttribPointer) {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
            glEnableVertexAttribArray(2);
        }

        if (glBindVertexArray) glBindVertexArray(0);
    }

    void Draw() const {
        if (vao == 0) return;
        if (glBindVertexArray) glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        if (glBindVertexArray) glBindVertexArray(0);
    }

    ~Mesh() {
        InitOpenGLExtensions();
        if (vao && glDeleteVertexArrays) glDeleteVertexArrays(1, &vao);
        if (vbo && glDeleteBuffers) glDeleteBuffers(1, &vbo);
        if (ebo && glDeleteBuffers) glDeleteBuffers(1, &ebo);
    }
};