#pragma once

#include <jsystem/texture.hpp>
#include <jsystem/vertex.hpp>
#include <glm/glm.hpp>

// Forward declare.
struct JMesh;
class JShader;

// Framebuffer ID.
using JFrameBufferID = GLuint;

// Buffer to a screen/texture. A lot of unfortunate singleton stuff.
class JFrameBuffer
{
    inline static JVertexUV quadVertices[] = {
        { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } }
    };
    inline static unsigned short quadIndices[] = { 0, 1, 2, 0, 2, 3 };
    inline static JFrameBuffer* mainBuffer;
    inline static JMesh* quad;
    inline static JShader* quadShader;
    JFrameBufferID fbo;
    JFrameBufferID rbo;
    JPtr<JTexture> tex;
    JPtr<JTexture> depthTex;
    int width;
    int height;

public:

    // Create a new framebuffer.
    JFrameBuffer(int width, int height);

    // Resize the framebuffer.
    void Resize(int width, int height);

    // Start drawing to current framebuffer.
    void Bind();

    // Cleanup framebuffer stuff.
    ~JFrameBuffer();

    // Create the main framebuffer. Also intializes quad mesh and shader.
    static void CreateMainBuffer(int width, int height);

    // Cleanup main buffer resources.
    static void CleanupMainBuffer();

    // Get main buffer.
    static JFrameBuffer& MainBuffer();

    // Call to start rendering.
    static void StartRender();

    // Call to end rendering.
    static void EndRender();

};