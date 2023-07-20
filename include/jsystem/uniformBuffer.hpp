#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

// Forward declare.
class JShader;
class JTexture;

// Actual information shared between shaders.
struct JUniformData
{
    glm::mat4 projection; // Projection matrix.
    glm::mat4 view; // View matrix.
    glm::mat4 model; // Model matrix.
};

using JUniformBufferID = GLuint;

// A buffer that stores a bunch of data shared between all shaders. Make sure to connect to at least one shader before sending data.
class JUniformBuffer
{
    JUniformBufferID buff; // Buffer handle.
    std::map<std::string, int> uniformLocations; // Locations for each uniform.

public:

    // Construct the buffer.
    JUniformBuffer();

    // Connect the buffer to a shader.
    void Connect(JShader& shader) const;

    // Get the location of a uniform.
    int GetUniformLocation(const std::string& name);

    // Set a 4x4 matrix uniform at a position.
    void SetMat4(const std::string& name, const glm::mat4& value);

    // Destroy the buffer.
    ~JUniformBuffer();

};