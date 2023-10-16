#pragma once

#include "fs.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Forward declare.
class JUniformBuffer;

using JShaderID = GLuint;
using JShaderProgramID = GLuint;

// A shader program.
class JShader
{
    std::map<std::string, int> uniformLocations; // Locations for each uniform.
    JShaderProgramID program; // ID for the shader program.

    // Friends :}
    friend class JUniformBuffer;

public:

    // Load a shader program. Has strings for shader uniform members and byte offsets.
    JShader(const JResPath& vertexPath, const JResPath& fragmentPath, const std::vector<std::pair<std::string, unsigned int>>& individualMembers, size_t structSize);

    // Compile a shader file. Returns it if it succeeds. Need to free shader if it succeeds.
    static std::optional<JShaderID> CompileShader(const JResPath& path, GLenum type);

    // Bind the shader.
    void Bind();

    // Get the location of a uniform.
    int GetUniformLocation(const std::string& name);

    // Set a bool uniform at a position.
    void SetBool(const std::string& name, bool value);

    // Set an int uniform at a position.
    void SetInt(const std::string& name, int value);

    // Set a 3d vector uniform at a position.
    void SetVec3(const std::string& name, const glm::vec3& value);

    // Set an int uniform at a position.
    void SetVec4(const std::string& name, const glm::vec4& value);

    // Set a 4x4 matrix uniform at a position.
    void SetMat4(const std::string& name, const glm::mat4& value);

    // Deconstruct the shader.
    ~JShader();

};

// Current shader.
extern JShader* CurrShader;