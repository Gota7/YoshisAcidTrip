#pragma once

#include "../yaml/helper.hpp"
#include "lua.hpp"
#include "zip.hpp"

// Plugin.
class PPlugin
{
    PZIP zip;
    std::unique_ptr<LuaCpp::Engine::LuaState> state;
    bool update = false;
    bool drawUI = false;
    bool render = false;
    bool deinit = false;
public:
    std::string name;
    std::string fullname;
    uint32_t version;

    // Create from a name.
    PPlugin(const std::string& name);
    
    // Run a function from the plugin. Returns status.
    // int RunFunc(const std::string& name);

    // To run every frame.
    void Update();

    // Draw UI.
    void DrawUI();

    // Render items.
    void Render();

    // Destructor.
    ~PPlugin();

};