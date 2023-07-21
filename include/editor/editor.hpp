#pragma once

#include <jsystem/renderBatch.hpp>
#include <jsystem/shader.hpp>
#include <jsystem/texture.hpp>
#include <jsystem/uniformBuffer.hpp>
#include <jsystem/vertex.hpp>

// Forward declare.
class JWindow;

// Main editor interface.
class EEditor
{
    JWindow& window;
    JUniformBuffer uniforms;
    JShader shdTest;
    JRenderBatch<JVertexUV> renderBatch;
    JTexture tex;

public:

    // Make a new editor.
    EEditor(JWindow& window);

    // Update the editor.
    void Update();

    // Render the editor.
    void Render();

};