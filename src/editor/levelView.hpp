#pragma once

#include "../jsystem/2d.hpp"
#include "../yaml/helper.hpp"

#define WIDTH_DEFAULT 1024
#define HEIGHT_DEFAULT 576

// View a level.
class ELevelView
{
    J2D view;
    bool shown = false;
public:

    // Create a new level view.
    ELevelView();

    // Draw the UI.
    void DrawUI(const std::string& levelName);

    // Render the level.
    void Render(YAML::Node& level);

};