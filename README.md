# Random Graphs Parkour

## Dependencies
The project contains git submodules, the CMakeLists.txt should update them but if it does not just do
```bash
git submodule update --init --recursive --checkout
```
this project uses different external libs:
* Glad <sub>(OpenGL 3.3)</sub>
* [Glfw3](https://github.com/glfw/glfw) <sub>(Window)</sub>
* [Nuklear](https://github.com/immediate-mode-ui/nuklear) <sub>(GUI)</sub>
* [Mathilda](https://github.com/Maxodie/Mathilda) <sub>(Math)</sub>

## Build
To build on Linux you can use the build.sh file that uses the cmake preset.<br>
The preset uses clang and C23.<br>
There are 2 configs: `debug` and `release`. The Debug preset will compile asserts and logs.
#### Default example of how to build with the preset
```bash
cmake --preset=debug -B build
cmake --build build
```
List of the custom build flags:
| FLAG             | desc (see details section)                      |
|------------------|-------------------------------------------------|
| RGP_LOGGER       | Enable the custom logger from yapping           |
| RGP_ASSERT_CORE  | Enable the custom asserts from existing    |

___
By default glfw is set up for Wayland and not X11 (see GLFW.cmake file)

## Flags Details
`RGP_LOGGER`
---
Enable different levels of logging, note that every macro authorizes basic format string and va_list.
| Name             | level                                                                               |
|------------------|-------------------------------------------------------------------------------------|
| `RGP_LOG_DEBUG(msg, ...)`    | Blue trace info, including initialization state or API version          |
| `RGP_LOG_SUCCESS(msg, ...)`  | Green info, used to inform on successful operations and giving dopamine |
| `RGP_LOG_WARNING(msg, ...)`  | Yellow warning with stack info, used for non critical errors            |
| `RGP_LOG_ERROR(msg, ...)`    | Red error with stack info, used for critical errors and being scary     |

`RGP_ASSERT_CORE`
---
Enable the `RGP_ASSERT(x, msg)` macro that takes a condition and an error message. <br>
Note that the error message will be printed with the logger.
