#pragma once

#include "../common.hpp"
#include "../yaml/helper.hpp"
#include "api.hpp"
#include "lua.hpp"
#include "zip.hpp"

#define PLUGIN_HOOK(var, hook) private: \
bool var = false; \
public: \
void hook() \
{ \
    ZoneScopedN("PPlugin::" #hook); \
    if (var) state = std::move(LUA_CTX.RunWithState(name + "." #var, std::move(state))); \
}

#define PLUGIN_HOOK_IMGUI(var, hook) private: \
bool var = false; \
public: \
void hook() \
{ \
    ZoneScopedN("PPlugin::" #hook); \
    LUA_IMGUI_CTX_VALID = true; \
    if (var) state = std::move(LUA_CTX.RunWithState(name + "." #var, std::move(state))); \
    LUA_IMGUI_CTX_VALID = false; \
}

// Plugin.
class PPlugin
{
    PZIP zip;
    std::unique_ptr<LuaCpp::Engine::LuaState> state;
    bool deinit = false;
public:
    std::string name;
    std::string fullname;
    std::string desc;
    std::string author;
    uint32_t version;

    // Create from a name.
    PPlugin(const std::string& name);
    
    // Run a function from the plugin. Returns status.
    // int RunFunc(const std::string& name);

    // Hooks.
    PLUGIN_HOOK(update, Update);
    PLUGIN_HOOK_IMGUI(drawUI, DrawUI);
    PLUGIN_HOOK(render, Render);

    // Destructor.
    ~PPlugin();

};