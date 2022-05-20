#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Logger.h"
#include "Tools.h"

class GameGui {
public:
    GameGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGuiIO &io = ImGui::GetIO();
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigDockingWithShift = true;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGui_ImplGlfw_InitForOpenGL(Tools::window, true);
        ImGui_ImplOpenGL3_Init();
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    };

    ~GameGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    };


    void drawwGUI() {
        GameGui::begin();
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f", io.Framerate);
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        ImGui::Text("S2W: (%s)", glm::to_string(screentoWorldPos_).c_str());
        ImGui::Text("S2C: (%s)", glm::to_string(screenColor_).c_str());
        ImGui::Text("Object id: (%d)", objectId_);

        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", glm::value_ptr(Tools::camera->pos), -10, 10);
        ImGui::SliderFloat3("Center", glm::value_ptr(Tools::camera->center), -10, 10);
        ImGui::SliderFloat3("Up", glm::value_ptr(Tools::camera->up), -1, 1);
        ImGui::End();
        GameGui::end();
    }


    void onEvent(const Event &event, double deltatime) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                if (key->key == GLFW_KEY_2 && key->press_release_repeat == 1) {
                    Tools::setFreeCamera(false);
                }
                if (key->key == GLFW_KEY_1 && key->press_release_repeat == 1) {
                    Tools::setFreeCamera(true);
                }
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                screentoWorldPos_ = Tools::screenToWorld(mouse->xPos, mouse->yPos);
                screenColor_ = Tools::screenToColor(mouse->xPos, mouse->yPos);
                objectId_ = Tools::colorToId(screenColor_);
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                break;
            }
            default:
                break;
        }
    }

    static void begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    };

    static void end() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Tools::window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    };
private:
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
    int objectId_{-1};

};

#endif //TFG_GAMEGUI_H
