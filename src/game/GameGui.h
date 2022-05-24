#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Utils.h"
#include "Logger.h"

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
        io.ConfigViewportsNoAutoMerge = true;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGui_ImplGlfw_InitForOpenGL(Injector::window, true);
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


    static void drawwGUI(const std::function<void()> &overlay = {}, const std::function<void()> &window = {}) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
                ImGui::Text("Mouse: (%.0f,%.0f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse: <invalid>");
            overlay();
        }
        ImGui::End();

        window();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Injector::window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
};

#endif //TFG_GAMEGUI_H
