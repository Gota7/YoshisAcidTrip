cmake_minimum_required(VERSION 3.10)
project(LSD)
include_directories(include)

set(BINARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})

# Some options.
option(BACKEND "Which backend to use. For PC platforms only, only GL supported atm." "GL")
option(OS "OS to compile for. Supports Linux, Windows, and Darwin." ${CMAKE_HOST_SYSTEM_NAME})
option(DEBUG "Allow for extra debug information." OFF)
option(TRACY "If to include the tracy profiler." OFF)
option(STATIC_BUILD "If to build the game without any external dependencies." OFF)

message("Backend is ${BACKEND}.")
message("OS is ${OS}.")
message("Debug is ${DEBUG}.")
message("Tracy is ${TRACY}.")
message("Static Build is ${STATIC_BUILD}.")

# Check for cross compiling.
if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES ${OS})
	set(CROSS_COMPILE ON)
else()
	set(CROSS_COMPILE OFF)
endif()

# Mac OS Shenanigans
if (NOT ${CROSS_COMPILE} AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(HOMEBREW_PREFIX "/usr/local"
	    CACHE PATH "Path to Homebrew installation")

	set(CMAKE_PREFIX_PATH
	    "${HOMEBREW_PREFIX}"
	    # These libraries are keg-only and not loaded into
	    # the root prefix by default (to avoid clashes).
	    "${HOMEBREW_PREFIX}/opt/lapack"
	    "${HOMEBREW_PREFIX}/opt/openblas"
	)

	list(TRANSFORM CMAKE_PREFIX_PATH APPEND "/include"
	     OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
	set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES "${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}")

	set(CMAKE_FIND_FRAMEWORK NEVER)
	set(CMAKE_FIND_APPBUNDLE NEVER)

	set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH FALSE)
	set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fpermissive -Wall -Wextra -Wno-unused-parameter")
if (${STATIC_BUILD} AND NOT WIN32) # Windows seems to act weird with link time optimization enabled.
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
if (${STATIC_BUILD} AND WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --static -mwindows")
endif()
add_subdirectory(subs/glm)
include_directories(subs/glm)
add_library(imgui STATIC
	subs/imgui/imgui.cpp
	subs/imgui/imgui_demo.cpp
	subs/imgui/imgui_draw.cpp
	subs/imgui/imgui_tables.cpp
	subs/imgui/imgui_widgets.cpp
#imgui/examples/imgui_impl_opengl3.cpp
#imgui_impl_raylib.cpp
)
include_directories(subs/imgui)

# Libs.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
if (${STATIC_BUILD})
    option(BUILD_SHARED_LIBS "" OFF)
	option(YAML_CPP_BUILD_TOOLS "" OFF)
	option(ZSTD_BUILD_PROGRAMS "" OFF)
	option(ZSTD_BUILD_STATIC "" ON)
	option(ZSTD_BUILD_SHARED "" OFF)
	set(BUILD_TESTING OFF)
    add_subdirectory(subs/glfw)
    include_directories(subs/glfw/include)
	add_subdirectory(subs/yaml-cpp)
    include_directories(subs/glfw/yaml-cpp/include)
	add_subdirectory(subs/zstd/build/cmake)
	include_directories(subs/zstd/lib)
	set(LIB_ZSTD libzstd_static)
else()
	find_path(ZSTD_INCLUDE_DIR NAMES zstd.h)
	find_library(ZSTD_LIBRARY_RELEASE NAMES zstd zstd_static)
	set(LIB_ZSTD ${ZSTD_LIBRARY_RELEASE})
endif()

# Add tracy if needed.
if (${TRACY})
    add_subdirectory(subs/tracy)
endif()

# Base libs.
set(LIBS "imgui")
set(LIBS "${LIBS} yaml-cpp")
set(LIBS "${LIBS} ${LIB_ZSTD}")

# Debug.
if (NOT ${DEBUG})
	add_compile_definitions(__NODEBUG__=1)
endif()

# OpenGL stuff.
add_compile_definitions(__GL__=1)
set(LIBS "${LIBS} glfw")

# Window libraries.
# if (WIN32)
# 	set(LIBS "${LIBS} Shlwapi")
# endif()

# Whitespace thing.
string(STRIP ${LIBS} LIBS)
string(REPLACE " " ";" LIBS ${LIBS})

# Source compilation.
include_directories("src")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Tracy client.
if (${TRACY})
    target_link_libraries(${PROJECT_NAME} ${LIBS} Tracy::TracyClient)
else()
    target_link_libraries(${PROJECT_NAME} ${LIBS})
endif()

# Resources.
if (${STATIC_BUILD})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
endif()