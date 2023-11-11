#pragma once

#include "../jsystem/renderBatch.hpp"
#include "../jsystem/shader.hpp"
#include "../jsystem/texture.hpp"
#include "../jsystem/uniformBuffer.hpp"
#include "../jsystem/vertex.hpp"
#include "../plugin/plugin.hpp"
#include "level.hpp"
#include "settings.hpp"
#include "styleEditor.hpp"
#include "welcome.hpp"

// Forward declare.
class JWindow;

// Main editor interface.
class EEditor
{
public:
    JWindow& window;
    JPtr<ELevel> level;
    ESettings settings;
    EStyleEditor styleEditor;
    JPtr<EWelcome> welcomeWindow;
    std::vector<JPtr<JTexture>> talkingFlowerFrames;
    std::vector<JPtr<PPlugin>> plugins;
    std::map<std::string, std::string> text; // For different langs.
    std::string statusText;

    // Debug stuff, delete later!
    JUniformBuffer uniforms;
    JShader shdTest;
    JRenderBatch<JVertexUV> renderBatch;
    JTexture tex;

private:

    // Draw the main bar.
    void DrawMainBar();

    // Draw about popup.
    void DrawAboutPopup();

    // Draw popups.
    void DrawPopups();

public:

    // Make a new editor.
    EEditor(JWindow& window);

    // Get number of talking flower frames.
    std::size_t NumTalkingFlowerFrames() const;

    // Get talking flower frame.
    JTexture& TalkingFlowerFrame(std::size_t ind);

    // Update the editor.
    void Update();

    // Draw editor UI.
    void DrawUI();

    // Render the editor.
    void Render();

};