#include <jsystem/uniformBuffer.hpp>

#include <common.hpp>
#include <jsystem/shader.hpp>

#define GL_UNIFORM_OFF(member) { #member, offsetof(JUniformData, member) }

JUniformBuffer::JUniformBuffer()
{
    ZoneScopedN("JUniformBuffer::JUniformBuffer");
    glGenBuffers(1, &buff);
    glBindBuffer(GL_UNIFORM_BUFFER, buff);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(JUniformData), nullptr, GL_STREAM_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, buff, 0, sizeof(JUniformData)); // One shared uniform buffer, always bind to port 0.
    uniformLocations =
    {
        GL_UNIFORM_OFF(projection),
        GL_UNIFORM_OFF(view),
        GL_UNIFORM_OFF(model),
    };
}

void JUniformBuffer::Connect(JShader& shader) const
{
    ZoneScopedN("JUniformBuffer::ConnectToShader");
    GLuint index = glGetUniformBlockIndex(shader.program, "sharedData");
    glUniformBlockBinding(shader.program, index, 0);
}

int JUniformBuffer::GetUniformLocation(const std::string& name)
{
    ZoneScopedN("JShader::GetUniformLocation");
    return uniformLocations[name];
}

void JUniformBuffer::SetMat4(const std::string& name, const glm::mat4& value)
{
    ZoneScopedN("JShader::SetMat4");
    glBindBuffer(GL_UNIFORM_BUFFER, buff);
    glBufferSubData(GL_UNIFORM_BUFFER, GetUniformLocation(name), sizeof(glm::mat4), &value);
}

JUniformBuffer::~JUniformBuffer()
{
    ZoneScopedN("JUniformBuffer::~JUniformBuffer");
    glDeleteBuffers(1, &buff);
}