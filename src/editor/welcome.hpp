#pragma once

#include <string>

// Forward declare.
class EEditor;

// Welcome window and tutorial thing.
class EWelcome
{
    EEditor& editor;
    std::string romfsPath;
    std::string basePath;
    std::string patchPath;
    std::string buildPath;
    std::string modName;
    unsigned int progress = 0;
    bool popped = false;
    bool fromRomfs = true;
public:

    // Make a new welcome window.
    EWelcome(EEditor& editor);

    // If the window is pop-up'd.
    bool Popped();

    // Pop open the window.
    void Popup();

    // Draw welcome UI.
    void DrawUI();

};