#pragma once

#include <jsystem/fs.hpp>
#include <jsystem/textureAnim.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

// Texture unit enum.
enum JTextureUnit
{
    TEXUNIT_3D,
    TEXUNIT_GENERAL_1,
    TEXUNIT_GENERAL_2,
    TEXUNIT_NOISE,
    TEXUNIT_COUNT
};

// Forward declare.
class JShader;

// Texture ID.
using JTexureID = GLuint;

// A standard texture.
class JTexture
{
    GLuint texture;
    std::unique_ptr<JTextureAnim> anim = nullptr;
    int width;
    int height;
    int numChannels;
    bool cubemap = false;

public:

    // Create a texture from a path. Will bind the texture.
    JTexture(const JResPath& path);

    // Create a cubemap texture. Will bind the texture.
    JTexture(const JResPath& right, const JResPath& left, const JResPath& top, const JResPath& bottom, const JResPath& front, const JResPath& back);

    // Create a blank texture. Will bind the texture.
    JTexture(int width, int height, bool depth = false);

    // Bind a texture to an index.
    void Bind(unsigned int texIndex);

    // Update texture animation.
    void UpdateTexAnim();

    // Get texture animation matrix.
    glm::mat4 GetTexAnimMtx();

    // Apply texture animation data.
    void ApplyTexAnim(JShader& shader, const std::string& animMtxName);

    // Deconstruct a texture.
    ~JTexture();

};