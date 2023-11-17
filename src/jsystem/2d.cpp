#include "2d.hpp"

J2D::J2D(unsigned int width, unsigned int height) : frameBuffer(width, height), renderBatch(SCREEN_MAX_NUM_PRIMS, JRenderBatchMode::Quads), lineTex(JResPath("tex/white.png")), widthLast(width), heightLast(height), width(width), height(height)
{
    ZoneScopedN("J2D::J2D");
}

JFrameBuffer& J2D::FrameBuffer()
{
    ZoneScopedN("J2D::FrameBuffer");
    return frameBuffer;
}

void J2D::FrameBegin()
{
    ZoneScopedN("J2D::FrameBegin");
    if (width != widthLast || height != heightLast)
    {
        widthLast = width;
        heightLast = height;
        frameBuffer.Resize(width, height);
    }
    frameBuffer.Bind();
    currZ = 0.0f;
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    renderBatch.Begin();
}

void J2D::FrameEnd()
{
    ZoneScopedN("J2D::FrameEnd");
    renderBatch.RenderAndFlush();
}

void J2D::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness)
{
    ZoneScopedN("J2D::DrawLine");
    if (lastTex != lineTex.GetID()) FlushIfNeeded(true);
    lineTex.Bind(0);
    lastTex = lineTex.GetID();
    JVertexColorAlphaUV a, b;
    a.pos = glm::vec3(p1, currZ);
    b.pos = glm::vec3(p2, currZ);
    a.color = b.color = color;
    a.uv.x = b.uv.x = 0.0f;
    a.uv.y = b.uv.y = 1.0f;
    renderBatch.AddLine(a, b, glm::vec3(0, 0, 1.0f), thickness); // Only need normal to camera which is always along XY plane.
    PushZ();
    FlushIfNeeded();
}

void J2D::DrawTex(JTexture& tex, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& srcPos, const glm::vec2& srcSize, const glm::vec4& tint)
{
    ZoneScopedN("J2D::DrawTex");
    if (tex.GetID() != lastTex) FlushIfNeeded(true);
    tex.Bind(0);
    lastTex = lineTex.GetID();
    JVertexColorAlphaUV bL, bR, tL, tR;
    bL.color = bR.color = tL.color = tR.color = tint;
    bL.pos = glm::vec3(pos, currZ);
    bR.pos = glm::vec3(pos + glm::vec2(size.x, 0.0f), currZ);
    tL.pos = glm::vec3(pos + glm::vec2(0.0f, size.y), currZ);
    tR.pos = glm::vec3(pos + size, currZ);
    float uvXMin = srcPos.x / tex.Width();
    float uvXMax = (srcPos.x + srcSize.x) / tex.Width();
    float uvYMin = srcPos.y / tex.Height();
    float uvYMax = (srcPos.y + srcSize.y) / tex.Height();
    bL.uv = glm::vec2(uvXMin, uvYMin);
    bR.uv = glm::vec2(uvXMax, uvYMin);
    tL.uv = glm::vec2(uvXMin, uvYMax);
    tR.uv = glm::vec2(uvXMax, uvYMax);
    renderBatch.AddVertex(bL);
    renderBatch.AddVertex(tL);
    renderBatch.AddVertex(tR);
    renderBatch.AddVertex(bR);
    PushZ();
    FlushIfNeeded(); 
}