#include "../api.hpp"
#include <imgui.h>

bool LUA_IMGUI_CTX_VALID = false;

#define LUA_CHECKIMGUI if (!LUA_IMGUI_CTX_VALID) { lua_pushliteral(L, "LUA@LSD: Imgui context is not valid in this scope."); lua_error(L); }

extern "C"
{

    LUA_FUNC(_imgui_showDemoWindow)
    {
        ZoneScoped;
        LUA_CHECKIMGUI;
        LUA_ARGS;
        LUA_ARGCCHECK(argv < 2);
        if (argv == 1) LUA_ARGCHECK(lua_isboolean(L, 1));
        bool val = (bool)((argv == 1) ? lua_toboolean(L, 1) : true);
        ImGui::ShowDemoWindow(&val);
        lua_pushboolean(L, val);
        return 1;
    }

}

std::shared_ptr<LuaLibrary> PLibImguiGet()
{
    std::shared_ptr<LuaLibrary> ret = std::make_shared<LuaLibrary>("imgui");
    ret->AddCFunction("showDemoWindow", _imgui_showDemoWindow);
    return ret;
}