#include <jsystem/shader.hpp>

#include <glm/ext.hpp>

JShader* CurrShader = nullptr;

std::optional<JShaderID> JShader::CompileShader(const JResPath& path, GLenum type)
{
    ZoneScopedN("JShader::CompileShader");

    // Load and compile shader.
    DBG_PRINT("SHADER@JUNO: Loading \"" << path.fullPath << "\".");
    if (!JFileSystem::FileExists(path))
    {
        DBG_PRINT("ERROR@JUNO: Shader file \"" << path.fullPath << "\" does not exist!");
        return std::nullopt;
    }
    JShaderID shader = glCreateShader(type);
    auto data = JFileSystem::ReadAllLines(path);
    const char* dataStr = data.c_str();
    glShaderSource(shader, 1, &dataStr, nullptr);
    glCompileShader(shader);
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        DBG_PRINT("ERROR@JUNO: SHADER::COMPILATION_FAILED, Shader: \"" << path.fullPath << "\"\n" << infoLog);
        return std::nullopt;
    }
    return std::optional<JShaderID>(shader);

}

JShader::JShader(const JResPath& vertexPath, const JResPath& fragmentPath, const std::vector<std::pair<std::string, unsigned int>>& individualMembers, size_t structSize)
{
    ZoneScopedN("JShader::JShader");

    // Init data.
    program = glCreateProgram();

    // Load and compile shader.
    auto vertexShader = CompileShader(vertexPath, GL_VERTEX_SHADER);
    if (!vertexShader) return;
    auto fragmentShader = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);
    if (!fragmentShader) return;

    // Connect shader to program.
    glAttachShader(program, vertexShader.value());
    glAttachShader(program, fragmentShader.value());
    glLinkProgram(program);
    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        DBG_PRINT("ERROR@JUNO: SHADER::PROGRAM::LINKING_FAILED\n" << infoLog);
    }
    glDeleteShader(vertexShader.value());
    glDeleteShader(fragmentShader.value());
    Bind();
    GLuint64 index = glGetUniformBlockIndex(program, "uniforms");
    glUniformBlockBinding(program, index, 2);

}

void JShader::Bind()
{
    ZoneScopedN("JShader::Bind");
    glUseProgram(program);
    CurrShader = this;
}

int JShader::GetUniformLocation(const std::string& name)
{
    ZoneScopedN("JShader::GetUniformLocation");
    auto item = uniformLocations.find(name);
    if (item == uniformLocations.end())
    {
        int ret = glGetUniformLocation(program, name.c_str());
        assert(ret != -1);
        uniformLocations[name] = ret;
        return ret;
    }
    else
    {
        return item->second;
    }
}

void JShader::SetBool(const std::string& name, bool value)
{
    ZoneScopedN("JShader::SetBool");
    glUniform1i(GetUniformLocation(name), (int)value);
}

void JShader::SetInt(const std::string& name, int value)
{
    ZoneScopedN("JShader::SetInt");
    glUniform1i(GetUniformLocation(name), value);
}

void JShader::SetVec3(const std::string& name, const glm::vec3& value)
{
    ZoneScopedN("JShader::SetVec3");
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void JShader::SetVec4(const std::string& name, const glm::vec4& value)
{
    ZoneScopedN("JShader::SetVec4");
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void JShader::SetMat4(const std::string& name, const glm::mat4& value)
{
    ZoneScopedN("JShader::SetMat4");
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

JShader::~JShader()
{
    ZoneScopedN("JShader::~JShader");
    glDeleteProgram(program);
}