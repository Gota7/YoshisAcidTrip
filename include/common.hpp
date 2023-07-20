#pragma once

#include "../subs/tracy/Tracy.hpp"
#include <iostream>

#define APP_TITLE "Yoshi's Acid Trip"
#define APP_MAJOR 0
#define APP_MINOR 0
#define APP_REVISION 0

#define SCR_WIDTH 1024
#define SCR_HEIGHT 576

#define DBG_PRINT(data) std::cout << data << std::endl;

#ifndef __NODEBUG__
    #define __DEBUG__
#endif