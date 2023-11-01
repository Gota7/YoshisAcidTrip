#pragma once

// Forward declare.
class EEditor;

// Welcome window and tutorial thing.
class EWelcome
{
    EEditor& editor;
    unsigned int progress = 0;
    bool popped = false;
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