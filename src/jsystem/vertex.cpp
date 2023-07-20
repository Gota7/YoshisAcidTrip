#include <jsystem/vertex.hpp>

#include <jsystem/vertexBuffer.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMPLEMENT_JVERTEX1(type, count, member) void type::SetShaderAttributes(JShader& shader) {} \
void type::SetBufferAttributes(JVertexBuffer& buffer) \
{ \
    ZoneScopedN("JVertex::SetBufferAttributes"); \
    buffer.Bind(); \
    glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member)); \
    glEnableVertexAttribArray(0); \
}

#define IMPLEMENT_JVERTEX2(type, count1, member1, count2, member2) void type::SetShaderAttributes(JShader& shader) {} \
void type::SetBufferAttributes(JVertexBuffer& buffer) \
{ \
    ZoneScopedN("JVertex::SetBufferAttributes"); \
    buffer.Bind(); \
    glVertexAttribPointer(0, count1, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member1)); \
    glVertexAttribPointer(1, count2, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member2)); \
    glEnableVertexAttribArray(0); \
    glEnableVertexAttribArray(1); \
}

#define IMPLEMENT_JVERTEX3(type, count1, member1, count2, member2, count3, member3) void type::SetShaderAttributes(JShader& shader) {} \
void type::SetBufferAttributes(JVertexBuffer& buffer) \
{ \
    ZoneScopedN("JVertex::SetBufferAttributes"); \
    buffer.Bind(); \
    glVertexAttribPointer(0, count1, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member1)); \
    glVertexAttribPointer(1, count2, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member2)); \
    glVertexAttribPointer(2, count3, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member3)); \
    glEnableVertexAttribArray(0); \
    glEnableVertexAttribArray(1); \
    glEnableVertexAttribArray(2); \
}

#define IMPLEMENT_JVERTEX4(type, count1, member1, count2, member2, count3, member3, count4, member4) void type::SetShaderAttributes(JShader& shader) {} \
void type::SetBufferAttributes(JVertexBuffer& buffer) \
{ \
    ZoneScopedN("JVertex::SetBufferAttributes"); \
    buffer.Bind(); \
    glVertexAttribPointer(0, count1, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member1)); \
    glVertexAttribPointer(1, count2, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member2)); \
    glVertexAttribPointer(2, count3, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member3)); \
    glVertexAttribPointer(3, count4, GL_FLOAT, GL_FALSE, sizeof(type), (void*)offsetof(type, member4)); \
    glEnableVertexAttribArray(0); \
    glEnableVertexAttribArray(1); \
    glEnableVertexAttribArray(2); \
    glEnableVertexAttribArray(3); \
}

IMPLEMENT_JVERTEX1(JVertex, 3, pos)
IMPLEMENT_JVERTEX2(JVertexUV, 3, pos, 2, uv)
IMPLEMENT_JVERTEX2(JVertexColor, 3, pos, 3, color)
IMPLEMENT_JVERTEX3(JVertexColorUV, 3, pos, 3, color, 2, uv)
IMPLEMENT_JVERTEX3(JVertexColorAlphaUV, 3, pos, 4, color, 2, uv)
IMPLEMENT_JVERTEX2(JVertexNormal, 3, pos, 3, normal)
IMPLEMENT_JVERTEX3(JVertexNormalUV, 3, pos, 3, normal, 2, uv)
IMPLEMENT_JVERTEX4(JVertexColorNormalUV, 3, pos, 3, color, 3, normal, 2, uv)