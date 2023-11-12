#include "../api.hpp"
#include <imgui.h>

bool LUA_IMGUI_CTX_VALID = false;

#define LUA_CHECKIMGUI if (!LUA_IMGUI_CTX_VALID) { lua_pushliteral(L, "LUA@LSD: Imgui context is not valid in this scope."); lua_error(L); }

extern "C"
{

    /**
     * Show the demo window of ImGui.
     * @name imgui.showDemoWindow
     * @param bool open: If the window show be open (optional).
     * @return bool: If the window is open.
    */
    LUA_FUNC(_imgui_showDemoWindow)
    {
        ZoneScoped;
        LUA_CHECKIMGUI;
        LUA_ARGS;
        LUA_ARGCCHECK(argv < 2);
        if (argv == 1) LUA_ARGCHECK(lua_isboolean(L, 1));
        bool val = (bool)((argv == 1) ? lua_toboolean(L, 1) : true);
        if (argv == 1) ImGui::ShowDemoWindow(&val);
        else ImGui::ShowDemoWindow();
        lua_pushboolean(L, val);
        return 1;
    }

}

std::shared_ptr<LuaLibrary> PLibImguiGet()
{
    ZoneScopedN("PLibImguiGet");
    std::shared_ptr<LuaLibrary> ret = std::make_shared<LuaLibrary>("imgui");
    ret->AddCFunction("showDemoWindow", _imgui_showDemoWindow);
    return ret;
}