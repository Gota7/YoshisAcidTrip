#pragma once

#include <jsystem/renderBatch.hpp>
#include <jsystem/shader.hpp>
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
    JRenderBatch<JVertexColor> renderBatch;

public:

    // Make a new editor.
    EEditor(JWindow& window);

    // Update the editor.
    void Update();

    // Render the editor.
    void Render();

};