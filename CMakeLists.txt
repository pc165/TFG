cmake_minimum_required(VERSION 3.21)
project(TFG)

set(CMAKE_CXX_STANDARD 14)
link_libraries(glfw)
link_libraries(GL)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

set(IMGUI_DIR src/lib/imgui)
set(IMGUI ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
set(IMGUI_BACKEND ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

file(GLOB_RECURSE SRC_FILES
        "src/engine/*.cpp"
        "src/engine/*.h"
        "src/engine/*.hpp"
        )
add_executable(TFG src/main.cpp ${SRC_FILES} ${IMGUI} ${IMGUI_BACKEND})