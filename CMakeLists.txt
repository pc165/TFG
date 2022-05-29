cmake_minimum_required(VERSION 3.21)
Include(FetchContent)
project(TFG)
set(CMAKE_CXX_STANDARD 17)

# GLEW and OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message("opengl found")
    message("include dir: ${OPENGL_INCLUDE_DIR}")
    message("link libraries: ${OPENGL_gl_LIBRARY}")
else (OPENGL_FOUND)
    message("opengl not found")
endif ()


# GLFW
FetchContent_Declare(
        glfw
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.7
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

#glm
FetchContent_Declare(
        glm
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# spdlog
FetchContent_Declare(
        spdlog
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.10.0
)
FetchContent_MakeAvailable(spdlog)

#imgui
FetchContent_Declare(
        imgui
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 2fa60be
)
FetchContent_MakeAvailable(imgui)

#glbinding
FetchContent_Declare(
        glbinding
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/cginternals/glbinding
        GIT_TAG master
)
FetchContent_MakeAvailable(glbinding)

#stb
FetchContent_Declare(
        stb
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG af1a5bc
)
FetchContent_MakeAvailable(stb)

set(IMGUI_DIR
        ${imgui_SOURCE_DIR})

FILE(GLOB LIBRARY_SOURCES
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

include_directories(
        ${glfw_INCLUDE_DIRS}
        ${glbinding_INCLUDE_DIRS}
        ${glm_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
        ${stb_SOURCE_DIR}
        src/game
        src/engine
        src/
)

#Source and Headers
file(GLOB_RECURSE SRC_FILES
        src/engine/*.cpp
        src/engine/*.h
        src/engine/*.hpp
        src/game/*.cpp
        src/game/*.h
        src/render/*.cpp
        src/render/*.h
        )


add_executable(TFG src/main.cpp ${SRC_FILES} ${LIBRARY_SOURCES})
target_link_libraries(TFG PRIVATE
        spdlog::spdlog glfw
        glbinding::glbinding
        glbinding::glbinding-aux
        ${OPENGL_gl_LIBRARY})

# warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_target_properties(TFG PROPERTIES COMPILE_FLAGS "-Weverything")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_target_properties(TFG PROPERTIES COMPILE_FLAGS "-Wall -Wno-unused-variable -Wno-deprecated-declarations")
endif ()