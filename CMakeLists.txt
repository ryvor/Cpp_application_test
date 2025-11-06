cmake_minimum_required(VERSION 3.20)

project(CustomTitleApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(custom_title_app
    src/main.cpp
    src/window_control.cpp
    src/window_control.h
    src/application.cpp
    src/application.h
    src/text_renderer.cpp
    src/text_renderer.h
    src/app_config.h
)

target_include_directories(custom_title_app PRIVATE src)

if (MSVC)
    target_compile_options(custom_title_app PRIVATE /W4 /permissive- /Zc:preprocessor)
else()
    target_compile_options(custom_title_app PRIVATE -Wall -Wextra -pedantic)
endif()

set(SDL2_IMPORTED_TARGETS "")

find_package(SDL2 2.0 QUIET CONFIG)

if (TARGET SDL2::SDL2)
    list(APPEND SDL2_IMPORTED_TARGETS SDL2::SDL2)
endif()
if (TARGET SDL2::SDL2main)
    list(APPEND SDL2_IMPORTED_TARGETS SDL2::SDL2main)
endif()

if (SDL2_IMPORTED_TARGETS STREQUAL "")
    find_package(SDL2 2.0 QUIET)
    if (SDL2_FOUND AND SDL2_LIBRARIES)
        target_include_directories(custom_title_app PRIVATE ${SDL2_INCLUDE_DIRS})
        target_link_libraries(custom_title_app PRIVATE ${SDL2_LIBRARIES})
    else()
        option(DOWNLOAD_SDL2 "Download SDL2 using FetchContent" ON)
        if (DOWNLOAD_SDL2)
            include(FetchContent)

            set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library" FORCE)
            set(SDL_STATIC ON CACHE BOOL "Build static SDL library" FORCE)
            set(SDL_TEST OFF CACHE BOOL "Build the SDL2 test library" FORCE)
            set(SDL_INSTALL OFF CACHE BOOL "Disable SDL installation" FORCE)

            FetchContent_Declare(
                sdl2
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-2.30.3
            )

            FetchContent_MakeAvailable(sdl2)

            list(APPEND SDL2_IMPORTED_TARGETS SDL2::SDL2-static)
            if (TARGET SDL2::SDL2main)
                list(APPEND SDL2_IMPORTED_TARGETS SDL2::SDL2main)
            endif()
        else()
            message(FATAL_ERROR "SDL2 could not be located and downloading is disabled")
        endif()
    endif()
endif()

if (NOT SDL2_IMPORTED_TARGETS STREQUAL "")
    target_link_libraries(custom_title_app PRIVATE ${SDL2_IMPORTED_TARGETS})
endif()
