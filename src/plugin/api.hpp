#include "../common.hpp"
#include "../lua/LuaCpp.hpp"

using namespace LuaCpp;
using namespace LuaCpp::Registry;
using namespace LuaCpp::Engine;

#define LUA_FUNC(name) static int name(lua_State* L)
#define LUA_ARGS int argv = lua_gettop(L)
#define LUA_ARGCCHECK(cond) if (!(cond)) { lua_pushliteral(L, "LUA@LSD: Bad arg count."); lua_error(L); }
#define LUA_ARGCHECK(cond) if (!(cond)) { lua_pushliteral(L, "LUA@LSD: Bad arg type."); lua_error(L); }

extern bool LUA_IMGUI_CTX_VALID;