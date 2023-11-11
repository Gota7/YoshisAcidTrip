/*
   MIT License

   Copyright (c) 2021 Jordan Vrtanoski

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   */

#ifndef LUACPP_LUACPP_HPP
#define LUACPP_LUACPP_HPP

#include "LuaVersion.hpp"
#include "Lua.hpp"
#include "LuaContext.hpp"
#include "LuaMetaObject.hpp"

#include "Engine/LuaState.hpp"
#include "Engine/LuaType.hpp"
#include "Engine/LuaTNil.hpp"
#include "Engine/LuaTString.hpp"
#include "Engine/LuaTBoolean.hpp"
#include "Engine/LuaTNumber.hpp"
#include "Engine/LuaTTable.hpp"
#include "Engine/LuaTUserData.hpp"

#include "Registry/LuaCompiler.hpp"
#include "Registry/LuaRegistry.hpp"
#include "Registry/LuaCodeSnippet.hpp"
#include "Registry/LuaLibrary.hpp"
#include "Registry/LuaCFunction.hpp"

#endif //LUACPP_LUACPP_HPP
