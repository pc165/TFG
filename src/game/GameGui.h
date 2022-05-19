#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "OpenGL.h"
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
        static int corner = 0;
        ImGuiIO &io = ImGui::GetIO();
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1) {
            const float PAD = 10.0f;
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Input Status", nullptr, window_flags)) {
            ImGui::Text("FPS: %.1f", io.Framerate);
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse Position: <invalid>");

            ImGui::Text("S2W: (%s)", glm::to_string(screentoWorldPos_).c_str());
            ImGui::Text("S2C: (%s)", glm::to_string(screenColor_).c_str());
            ImGui::Text("Object id: (%d)", objectId_);
        }
        ImGui::End();

        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", glm::value_ptr(Tools::camera->pos), -10, 10);
        ImGui::SliderFloat3("Center", glm::value_ptr(Tools::camera->center), -10, 10);
        ImGui::SliderFloat3("Up", glm::value_ptr(Tools::camera->up), -1, 1);
        ImGui::End();
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

    void begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    };

    void end() {
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
