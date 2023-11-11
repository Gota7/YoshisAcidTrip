#include "lua.hpp"

#include "api/imgui.cc"

LuaCpp::LuaContext LUA_CTX;

void PLua::InitAPI()
{
    ZoneScopedN("PLua::InitAPI");
    auto imgui = PLibImguiGet();
    LUA_CTX.AddLibrary(imgui);
}