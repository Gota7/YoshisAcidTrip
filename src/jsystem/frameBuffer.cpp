#include <jsystem/frameBuffer.hpp>

#include <jsystem/mesh.hpp>
#include <jsystem/shader.hpp>
#include <glm/ext.hpp>

JFrameBuffer::JFrameBuffer(int width, int height)
{
    ZoneScopedN("JFrameBuffer::JFrameBuffer");
    glGenFramebuffers(1, &fbo);
    glGenRenderbuffers(1, &rbo);
    Resize(width, height);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        DBG_PRINT("FRAMEBUFFER@JUNO: Frame buffer incomplete!");
        return;
    }
}

void JFrameBuffer::Resize(int width, int height)
{
    ZoneScopedN("JFrameBuffer::Resize");
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    tex = JPtrMake(JTexture, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texture, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void JFrameBuffer::Bind()
{
    ZoneScopedN("JFrameBuffer::Bind");
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

JTexture& JFrameBuffer::GetTex()
{
    ZoneScopedN("JFrameBuffer::GetTex");
    return *tex;
}

JFrameBuffer::~JFrameBuffer()
{
    ZoneScopedN("JFrameBuffer::~JFrameBuffer");
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
}

void JFrameBuffer::CreateMainBuffer(int width, int height)
{
    ZoneScopedN("JFrameBuffer::CreateMainBuffer");
    JAlloc(mainBuffer, JFrameBuffer, width, height);
    JAlloc(quad, JMesh, JPtrMake(JVertexBuffer,
        quadVertices,
        sizeof(quadVertices),
        quadIndices,
        sizeof(quadIndices),
        sizeof(quadVertices) / sizeof(quadVertices[0])
    ), 0);
    JVertexUV::SetBufferAttributes(*quad->buffer);
    JAlloc(quadShader, JShader, JResPath("shd/quad.vert"), JResPath("shd/quad.frag"), {}, 0);
    JVertexUV::SetShaderAttributes(*quadShader);
    quadShader->SetInt("ScreenTexture", 0);
}

void JFrameBuffer::CleanupMainBuffer()
{
    ZoneScopedN("JFrameBuffer::CleanupMainBuffer");
    JFree(mainBuffer);
    JFree(quad);
    JFree(quadShader);
}

JFrameBuffer& JFrameBuffer::MainBuffer()
{
    ZoneScopedN("JFrameBuffer::MainBuffer");
    return *mainBuffer;
}

void JFrameBuffer::StartRender()
{
    ZoneScopedN("JFrameBuffer::StartRender");
    mainBuffer->Bind();
    glEnable(GL_DEPTH_TEST);
}

void JFrameBuffer::EndRender()
{
    ZoneScopedN("JFrameBuffer::EndRender");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    quadShader->Bind();
    mainBuffer->tex->Bind(0);
    quad->Render();
}