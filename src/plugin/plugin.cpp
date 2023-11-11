#include "plugin.hpp"

#include "../jsystem/fs.hpp"
#include "../jsystem/mem.hpp"
#include "api.hpp"

PPlugin::PPlugin(const std::string& name) : zip(JResPath("plg/" + name + ".zip").fullPath), name(name)
{
    auto yaml = zip.String("plugin.yaml");
    auto code = zip.String("init.lua");
    if (!yaml)
    {
        DBG_PRINT("PLUGIN@LSD: Invalid plugin \"" + name + "\".");
        return;
    }
    YAML::Node root = YAML::Load(*yaml);
    fullname = root["Name"].as<std::string>();
    version = root["Version"].as<uint32_t>();
    if (code)
    {
        LUA_CTX.CompileString(name + ".init", *code);
        state = std::move(LUA_CTX.RunWithState(name + ".init", LUA_CTX.newStateFor(name + ".init")));
    }
    code = zip.String("update.lua");
    if (code)
    {
        LUA_CTX.CompileString(name + ".update", *code);
        update = true;
    }
    code = zip.String("drawUI.lua");
    if (code)
    {
        LUA_CTX.CompileString(name + ".drawUI", *code);
        drawUI = true;
    }
    code = zip.String("render.lua");
    if (code)
    {
        LUA_CTX.CompileString(name + ".render", *code);
        render = true;
    }
    code = zip.String("deinit.lua");
    if (code)
    {
        LUA_CTX.CompileString(name + ".deinit", *code);
        deinit = true;
    }
}

// int PPlugin::RunFunc(const std::string& name)
// {
//     JPtr<LuaCpp::Engine::LuaState> L = LUA_CTX.newStateFor(this->name);
//     lua_getglobal(L->getState(), name.c_str());
//     // You would push args here.
//     return lua_pcall(L->getState(), 0, 0, 0); // Number of args, number of results, error function.
// }

void PPlugin::Update()
{
    ZoneScopedN("PPlugin::Update");
    if (update) state = std::move(LUA_CTX.RunWithState(name + ".update", std::move(state)));
}

void PPlugin::DrawUI()
{
    ZoneScopedN("PPlugin::DrawUI");
    LUA_IMGUI_CTX_VALID = true;
    if (drawUI) state = std::move(LUA_CTX.RunWithState(name + ".drawUI", std::move(state)));
    LUA_IMGUI_CTX_VALID = false;
}

void PPlugin::Render()
{
    ZoneScopedN("PPlugin::Render");
    if (render) state = std::move(LUA_CTX.RunWithState(name + ".render", std::move(state)));
}

PPlugin::~PPlugin()
{
    ZoneScopedN("PPlugin::~PPlugin");
    if (deinit) state = std::move(LUA_CTX.RunWithState(name + ".deinit", std::move(state)));
}