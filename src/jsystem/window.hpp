#pragma once

#include "../texture/format.hpp"
#include "mem.hpp"
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui/imgui_impl_opengl3.hpp>
#include <map>
#include <string>

using JWindowCallback = std::function<void()>;

#define DELTA_TIME_MAX 0.2

// For a window. This should be a singleton, it is NOT expected to have multiple instances.
class JWindow
{
    std::vector<std::string> fontKeys;
    std::map<std::string, ImFont*> fonts;
    std::map<std::string, int> revFonts;
    std::string oldFont = "Default";
    float oldFontSize = 12.0f;
    GLFWimage icon;
    GLFWwindow* window;
    double currentFrame = 0; // Current time in seconds.
    double lastFrame = 0; // Last frame in seconds.
    double deltaTime = 1.0 / 60; // Time between last frame and this one.
    double lastSecond = 0; // Last frame that was the seconds count.
    glm::ivec2 posLast = glm::ivec2(0, 0); // Last window position.
    glm::ivec2 sizeLast = glm::ivec2(0, 0); // Last window size.
    int width;
    int height;
    int fps = 60; // Current FPS.
    int frameCounter = 0; // Frame counter for each second.
    unsigned int targetFps = 0; // Target frames per seconds.
    bool fullscreenKeyDown = false;
    bool fullscreen = false;

    // Update frame information and delta time.
    void UpdateFrame();

    // Work for actually scanning fonts.
    void DoScanFonts();

public:
    std::string currFont = "Default"; // Current font in use.
    float currFontSize = 12.0f;
    bool needsRender = false; // If the window needs to render. Only used if FPS is set to 1.
    bool needsFontScanned = true; // If available fonts need to be scanned.

    // Make a new window.
    JWindow(const std::string& title);

    // Window main loop with custom main loop callback.
    void Main(const JWindowCallback& mainCallback = JWindowCallback(nullptr), const JWindowCallback& renderCallback = JWindowCallback(nullptr), const JWindowCallback& drawUICallback = JWindowCallback(nullptr));

    // Set the size of the window.
    void SetSize(int width, int height);

    // Get size of the window.
    void GetSize(int& width, int& height) const;

    // Time since last frame.
    float DeltaTime() const;

    // Set the FPS. Set to 0 to use VSYNC. Set to 1 to only draw on needs render flag set.
    void SetFPS(unsigned int fps = 0);

    // Get the FPS. Returns 0 for VSYNC. Returns 1 to only draw on needs render flag set.
    unsigned int GetFPS() const;

    // Get the actual FPS the system is running at given the current frame.
    float GetActualFPS() const;

    // Get a font by name. If it does not exist, get the default one.
    ImFont* GetFont(const std::string& name);

    // Get a font name by index. Returns empty string if invalid.
    std::string& GetFontByInd(int ind);

    // Get index by font.
    int GetIndByFont(const std::string& name);

    // Get font count.
    int GetFontCount() const;

    // Close window.
    ~JWindow();

};