#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include "OpenGL.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Utils.h"
#include "Logger.h"

class GameGui {
public:
    GameGui();

    ~GameGui();

    static void drawOverlay(const std::function<void()> &overlay) {
        static int corner = 0;
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
        ImGuiIO &io = ImGui::GetIO();

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Input Status", nullptr, window_flags)) {
            ImGui::Text("FPS: %.1f", io.Framerate);
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse: (%.0f,%.0f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse: <invalid>");
            overlay();
            ImGui::End();
        }
    };

    static void drawDockSpace() {
//        ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());
//        glfwMakeContextCurrent(Injector::window);
    }

    static void HelpMarker(const char *desc) {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    static void drawwGUI(const std::function<void()> &overlay, const std::function<void()> &window) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GameGui::drawDockSpace();
        GameGui::drawOverlay(overlay);
        window();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Injector::window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
};

#endif //TFG_GAMEGUI_H
