#pragma once

#include "../lua/LuaCpp.hpp"

// Global lua configuration.
namespace PLua
{

    // Initialize the API.
    void InitAPI();
    
}

// Lua context.
extern LuaCpp::LuaContext LUA_CTX;