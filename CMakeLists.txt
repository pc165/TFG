cmake_minimum_required(VERSION 3.21)
project(TFG)

set(CMAKE_CXX_STANDARD 14)
link_libraries(glfw)
link_libraries(GL)

set(IMGUI_DIR src/lib/imgui)
set(IMGUI ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
set(IMGUI_BACKEND ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_executable(TFG src/main.cpp ${IMGUI} ${IMGUI_BACKEND})
