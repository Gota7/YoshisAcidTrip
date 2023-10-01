#pragma once

#include <jsystem/texture.hpp>
#include <glm/glm.hpp>

// Forward declare.
class JFrameBuffer;

// Screen to draw on.
class ZScreen
{
    unsigned int widthLast;
    unsigned int heightLast;
public:
    glm::vec4 bgColor;
    unsigned int width;
    unsigned int height;

    // Create a new screen.
    ZScreen(unsigned int width, unsigned int height);

    // Get the render target.
    JFrameBuffer& FrameBuffer();

    // Begin drawing a new frame.
    void FrameBegin();

    // End drawing a new frame.
    void FrameEnd();

    // Draw a line.
    void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0f);

    // Draw a texture.
    void DrawTex(const JTextur& tex, const glm::vec2& pos)
    {
        DrawTex(tex, pos, glm::vec2(tex.width, tex.height));
    }

    // Draw a texture.
    void DrawTex(const JTexture& tex, const glm::vec2& pos, const glm::vec2& size)
    {
        DrawTex(tex, pos, size, glm::vec2(0.0f), glm::vec2(tex.width, tex.height));
    }

    // Draw a texture.
    void DrawTex(const JTexture& tex, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& srcPos, const glm::vec2& srcSize);

};