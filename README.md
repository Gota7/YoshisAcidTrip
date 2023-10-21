# Yoshi's Acid Trip
An editor for Super Mario Bros. Wonder.

## When Will It Be Ready?
I don't know. Currently the plan is to get ROMFS conversion working and tackle problems as I get there. After the conversion system works, I'll work on understanding and viewing the levels and go from there.

## More Info Coming Soon
[Discord](https://discord.gg/qtxgeNCyD3)

## Code Organization
Each folder/lib in the `src` folder is used for a particular task and is assigned a letter for all structs and namespaces to begin with. The headers for each file are also located in `include`. There are outliers such as `imgui` and `glad` for implementing important libs taken from elsewhere.

| Library | Letter | Description |
| ------- | ------ | ----------- |
| Bin | B | Binary stream reading/writing. |
| Editor | E | Main editor UI code. |
| Hoganshi | H | Used for rendering tiles. |
| JSystem | J | Juno system. Slightly platform-independent rendering system for Gota's Galactic Journey. This version has much stripped out to be just for OpenGL now. |
| Mario | M | For Mario-Wonder specific formats and data. |
| Texture | T | For loading texture data. |
| Zu | Z | 2d rendering library for OpenGL. |

## Building
Building is done with docker using the [scuba](https://github.com/JonathonReinhart/scuba) build system. Install docker and scuba on your system for release builds. For example, to build for Windows, type `scuba windows` in a CMD cd'd to the folder of this editor.

| Command | Description |
| ------- | ----------- |
| `scuba clean` | Remove built binaries and build files. |
| `scuba linux` | Build for linux. |
| `scuba windows` | Build for windows. |

The executables will be located in the `bin` folder.

### Debug Building
You may need your own non-docker based builds for testing. Of course you need to have a working compiler and cmake installed as well as GLFW. The below instructions instruct you how to do this:

1. Optionally write the date to `BUILDTIME`. This is used to help with support for release builds, but for debug builds has little purpose other than your own book keeping. On linux, this is done with `date > res/BUILDTIME`.
2. Create the `build` folder if needed and CD to it. On linux, this is done with `mkdir -p build && cd build`.
3. Configure the project with necessary build flags. This is done with `cmake .. FLAGS`, where `FLAGS` are the build flags specified in the table below. An example command for this is `cmake .. -DBACKEND="GL" -DDEBUG=ON`.
4. Build the editor. This is done with `cmake --build . -j8`. You should change the number after `j` to reflect how many threads your CPU supports.
5. If all went well, you the project should have been built. Enjoy!

Supported flags:
| Flag | Options | Default | Description |
| ---- | ------- | ------- | ----------- |
| -DBACKEND | GL | -DBACKEND=GL | Backend used for the graphics API. Only OpenGL is supported at this time. |
| -DOS | Darwin/Linux/Windows | -DOS=${CMAKE_HOST_SYSTEM} | Which OS to build for. By default, it is the one of the system you are building on. Note that cross compiling is not just simply toggling this, you most likely will need a toolchain for your platform as well (Ex: `-DCMAKE_TOOLCHAIN_FILE=toolchains/windows.cmake`). |
| -DDEBUG | ON/OFF | -DDEBUG=OFF | Enable debugging features. |
| -DTRACY | ON/OFF | -DTRACY=OFF | Enable profiling using the [tracy](https://github.com/wolfpld/tracy) profiler. |
| -DSTATIC_BUILD | ON/OFF | -DSTATIC_BUILD=OFF | Use libraries from subdirectories rather than ones on the system. Should be enabled for release builds, disabling makes debug builds faster. |