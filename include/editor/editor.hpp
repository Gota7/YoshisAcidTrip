#pragma once

#include <editor/level.hpp>
#include <editor/settings.hpp>
#include <editor/styleEditor.hpp>
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
    JPtr<ELevel> level;
    ESettings settings;
    EStyleEditor styleEditor;

    // Debug stuff, delete later!
    JUniformBuffer uniforms;
    JShader shdTest;
    JRenderBatch<JVertexUV> renderBatch;
    JTexture tex;

    // Draw the main bar.
    void DrawMainBar();

    // Draw about popup.
    void DrawAboutPopup();

    // Draw popups.
    void DrawPopups();

public:

    // Make a new editor.
    EEditor(JWindow& window);

    // Update the editor.
    void Update();

    // Draw editor UI.
    void DrawUI();

    // Render the editor.
    void Render();

};