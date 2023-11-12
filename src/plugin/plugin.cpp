#include "plugin.hpp"

#include "../jsystem/fs.hpp"
#include "../jsystem/mem.hpp"
#include <set>

#define PLUGIN_COMP_HOOK(var) \
code = zip.String(#var ".hook.lua"); \
libBlacklist.emplace(#var ".hook.lua"); \
if (code) \
{ \
    LUA_CTX.CompileString(name + "." #var, *code); \
    var = true; \
}

PPlugin::PPlugin(const std::string& name) : zip(JResPath("plg/" + name + ".zip").fullPath), name(name)
{
    auto yaml = zip.String("plugin.yaml");
    auto code = zip.String("init.hook.lua");
    if (!yaml)
    {
        DBG_PRINT("PLUGIN@LSD: Invalid plugin \"" + name + "\".");
        return;
    }

    // Plugin info.
    YAML::Node root = YAML::Load(*yaml);
    fullname = root["Name"].IsDefined() ? root["Name"].as<std::string>() : name ;
    desc = root["Desc"].IsDefined() ? root["Desc"].as<std::string>() : "Some plugin.";
    author = root["Author"].IsDefined() ? root["Author"].as<std::string>() : "Unknown Author";
    version = root["Version"].IsDefined() ? root["Version"].as<uint32_t>() : 0;

    // Initial code.
    if (code)
    {
        LUA_CTX.CompileString(name + ".init", *code);
        state = std::move(LUA_CTX.RunWithState(name + ".init", LUA_CTX.newStateFor(name + ".init")));
    }
    else state = std::move(LUA_CTX.newState()); // Even though libs also run, we must guarantee init runs first.
    std::set<std::string> libBlacklist;
    libBlacklist.emplace("init.hook.lua");
    PLUGIN_COMP_HOOK(update);
    PLUGIN_COMP_HOOK(drawUI);
    PLUGIN_COMP_HOOK(render);
    PLUGIN_COMP_HOOK(deinit);
    for (auto file : zip.FileList())
    {
        if (!file.ends_with(".lua") || libBlacklist.find(file) != libBlacklist.end()) continue;
        code = zip.String(file);
        LUA_CTX.CompileString(name + "." + file, *code);
        state = std::move(LUA_CTX.RunWithState(name + "." + file, std::move(state)));
    }
}

// int PPlugin::RunFunc(const std::string& name)
// {
//     JPtr<LuaCpp::Engine::LuaState> L = LUA_CTX.newStateFor(this->name);
//     lua_getglobal(L->getState(), name.c_str());
//     // You would push args here.
//     return lua_pcall(L->getState(), 0, 0, 0); // Number of args, number of results, error function.
// }

PPlugin::~PPlugin()
{
    ZoneScopedN("PPlugin::~PPlugin");
    if (deinit) state = std::move(LUA_CTX.RunWithState(name + ".deinit", std::move(state)));
}