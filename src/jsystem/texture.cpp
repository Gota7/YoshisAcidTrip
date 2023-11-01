#include <jsystem/texture.hpp>

#include <jsystem/shader.hpp>

JTexture::JTexture(const TFormat& texData)
{
    ZoneScopedN("JTexture::JTexture");

    // Create tex.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texData.Properties(width, height, numChannels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.DataPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

}

JTexture::JTexture(const JResPath& path)
{
    ZoneScopedN("JTexture::JTexture");

    // Create tex.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    auto texData = TLoader::LoadTexture(path);
    if (texData)
    {
        texData->Properties(width, height, numChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData->DataPtr());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        DBG_PRINT("Failed to load texture " << path.fullPath << ".");
    }

}

JTexture::JTexture(const TFormat& right, const TFormat& left, const TFormat& top, const TFormat& bottom, const TFormat& front, const TFormat& back)
{
    ZoneScopedN("JTexture::JTexture");
    auto LoadFace = [](const TFormat& texData, GLenum face)
    {
        int width, height, numChannels;
        texData.Properties(width, height, numChannels);
        glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.DataPtr());
    };
    cubemap = true;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    LoadFace(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadFace(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadFace(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadFace(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadFace(front, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadFace(back, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

JTexture::JTexture(const JResPath& right, const JResPath& left, const JResPath& top, const JResPath& bottom, const JResPath& front, const JResPath& back)
{
    ZoneScopedN("JTexture::JTexture");
    auto LoadFace = [](const JResPath& path, GLenum face)
    {
        auto texData = TLoader::LoadTexture(path);
        if (texData)
        {
            int width, height, numChannels;
            texData->Properties(width, height, numChannels);
            glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData->DataPtr());
        }
        else
        {
            DBG_PRINT("Cubemap texture failed to load at path: " << path.fullPath);
        }
    };
    cubemap = true;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    LoadFace(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadFace(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadFace(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadFace(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadFace(front, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadFace(back, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

JTexture::JTexture(int width, int height, bool depth) : width(width), height(height), numChannels(1)
{
    ZoneScopedN("JTexture::JTexture");
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (depth) glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void JTexture::Bind(unsigned int texIndex)
{
    ZoneScopedN("JTexture::Bind");
    glActiveTexture(GL_TEXTURE0 + texIndex); // Set the active texture unit and bind texture to it. Textures are attached to texture units, not shaders.
    glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texture);
}

void JTexture::UpdateTexAnim()
{
    ZoneScopedN("JTexture::UpdateTexAnim");
    if (anim) anim->Update();
}

glm::mat4 JTexture::GetTexAnimMtx()
{
    ZoneScopedN("JTexture::GetTexAnimMtx");
    return anim ? glm::mat4(anim->GetMtx()) : glm::mat4(1.0f);
}

void JTexture::ApplyTexAnim(JShader& shader, const std::string& animMtxName)
{
    ZoneScopedN("JTexture::ApplyTexAnim");
    shader.SetMat4(animMtxName.c_str(), anim ? glm::mat4(anim->GetMtx()) : glm::mat4(1.0f));
}

int JTexture::Width() const
{
    ZoneScopedN("JTexture::Width");
    return width;
}

int JTexture::Height() const
{
    ZoneScopedN("JTexture::Height");
    return height;
}

GLuint JTexture::GetID() const
{
    ZoneScopedN("JTexture::GetID");
    return texture;
}

JTexture::~JTexture()
{
    ZoneScopedN("JTexture::~JTexture");
    glDeleteTextures(1, &texture);
}