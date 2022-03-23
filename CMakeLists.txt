cmake_minimum_required(VERSION 3.21)
project(TFG)
set(CMAKE_CXX_STANDARD 17)

# GLEW and OpenGL
link_libraries(glfw)
link_libraries(GL)
find_package(GLEW REQUIRED)
link_libraries(${GLEW_LIBRARIES})

# spdlog
Include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)

#imgui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG origin/docking
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR
        ${imgui_SOURCE_DIR})

FILE(GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

include_directories(
        ${GLEW_INCLUDE_DIRS}
        ${imgui_SOURCE_DIR}
        src/engine
        src/game
        src/
)

#Source and Headers
file(GLOB_RECURSE SRC_FILES
        src/engine/*.cpp
        src/engine/*.h
        src/game/*.cpp
        src/game/*.h
        )

add_executable(TFG src/main.cpp ${SRC_FILES} ${IMGUI_SOURCES})
target_link_libraries(TFG PRIVATE spdlog::spdlog)

# warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_target_properties(TFG PROPERTIES COMPILE_FLAGS "-Weverything")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_target_properties(TFG PROPERTIES COMPILE_FLAGS "-Wall")
endif ()