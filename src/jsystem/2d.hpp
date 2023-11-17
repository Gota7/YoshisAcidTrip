#pragma once

#include "frameBuffer.hpp"
#include "renderBatch.hpp"
#include "texture.hpp"
#include <glm/glm.hpp>

#define SCREEN_MAX_NUM_PRIMS 200

// 2D render target. Note that this requires the use of the J2d shader!
class J2D
{
    JFrameBuffer frameBuffer;
    JRenderBatch<JVertexColorAlphaUV> renderBatch;
    JTexture lineTex;
    unsigned int widthLast;
    unsigned int heightLast;
    float currZ = 0.0f;
    unsigned int currPrim = 0;
    GLuint lastTex; // TODO: FIGURE OUT GOOD WAY TO SWITCH TEXTURES!! Also fix breaking rule of no using GL stuff outside its JSystem wrapper.

    // Flush if needed.
    void FlushIfNeeded(bool force = false)
    {
        if (force || ++currPrim >= SCREEN_MAX_NUM_PRIMS)
        {
            glBindTexture(GL_TEXTURE_2D, lastTex);
            renderBatch.RenderAndFlush();
            renderBatch.Begin();
            currPrim = 0;
        }
    }

    // Push Z forward for making next item more on top.
    void PushZ()
    {
        currZ += 0.1f;
    }

public:
    glm::vec4 bgColor = glm::vec4(0.0f);
    unsigned int width;
    unsigned int height;

    // Create a new 2d target.
    J2D(unsigned int width, unsigned int height);

    // Get the render target.
    JFrameBuffer& FrameBuffer();

    // Begin drawing a new frame. Make sure that you are using the J2d shader!
    void FrameBegin();

    // End drawing a new frame.
    void FrameEnd();

    // Draw a line.
    void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0f);

    // Draw a texture.
    void DrawTex(JTexture& tex, const glm::vec2& pos, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f })
    {
        DrawTex(tex, pos, glm::vec2(tex.Width(), tex.Height()), tint);
    }

    // Draw a texture.
    void DrawTex(JTexture& tex, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f })
    {
        DrawTex(tex, pos, size, glm::vec2(0.0f), glm::vec2(tex.Width(), tex.Height()), tint);
    }

    // Draw a texture.
    void DrawTex(JTexture& tex, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& srcPos, const glm::vec2& srcSize, const glm::vec4& tint = { 1.0f, 1.0f, 1.0f, 1.0f });

};