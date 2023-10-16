#pragma once

#include <glm/glm.hpp>

// Forward declare.
class JShader;
class JVertexBuffer;

// A vertex with members based on what is needed.
// Call SetShaderAttributes after shader construction but before intialization.
// Call SetBufferAttributes after vertex buffer construction.
#define MAKE_JVERTEX(name, members) struct name \
{   \
    members;\
\
    static void SetShaderAttributes(JShader& shader); \
\
    static void SetBufferAttributes(JVertexBuffer& buffer); \
}

MAKE_JVERTEX(JVertex, glm::vec3 pos);
MAKE_JVERTEX(JVertexColor, glm::vec3 pos; glm::vec3 color);
MAKE_JVERTEX(JVertexColorUV, glm::vec3 pos; glm::vec3 color; glm::vec2 uv);
MAKE_JVERTEX(JVertexColorAlphaUV, glm::vec3 pos; glm::vec4 color; glm::vec2 uv);
MAKE_JVERTEX(JVertexNormal, glm::vec3 pos; glm::vec3 normal);
MAKE_JVERTEX(JVertexNormalUV, glm::vec3 pos; glm::vec3 normal; glm::vec2 uv);
MAKE_JVERTEX(JVertexUV, glm::vec3 pos; glm::vec2 uv);
MAKE_JVERTEX(JVertexColorNormalUV, glm::vec3 pos; glm::vec3 color; glm::vec3 normal; glm::vec2 uv);