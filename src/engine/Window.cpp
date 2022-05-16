#include <imgui.h>
#include "Window.h"
#include "Utils.h"
#include "Tools.h"

GLFWwindow *InitWindow(const char *title, int width, int height) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    LOG_DEBUG("Window created {}", fmt::ptr(window));
    if (window == nullptr) {
        LOG_CRITICAL("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGL()) {
        LOG_CRITICAL("Failed to initialize GLEW");
        glfwTerminate();
    }
    glDebugMessageCallback(errorOccurredGL, nullptr);

    // enable Z-test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    auto winProp = new WindowStruct{width, height};
    glfwSetWindowUserPointer(window, winProp);

    Tools::window = window;
    Tools::windowStruct = winProp;
    return window;
}

void ConfigureEvents(GLFWwindow *window) {
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;
        WindowResizeEvent event(EventType::WindowResize, width, height);
        data.eventCallback(event);
    });

    glfwSetWindowPosCallback(window, [](GLFWwindow *window, int xpos, int ypos) {
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        WindowPositionEvent event(EventType::WindowPosition, xpos, ypos);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event(EventType::WindowClose);
        data.shouldClose = true;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureKeyboard)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        KeyEvent event(EventType::Key, key, 0);
        if (action == GLFW_PRESS)
            event.press_release_repeat = 0;
        else if (action == GLFW_RELEASE)
            event.press_release_repeat = 1;
        else
            event.press_release_repeat = 2;
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseButtonEvent event(EventType::MouseButton, button, action == GLFW_PRESS ? 0 : 1);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseScrollEvent event(EventType::MouseScrolled, (float) xOffset, (float) yOffset);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xPos, double yPos) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseMoveEvent event(EventType::MouseMoved, (float) xPos, (float) yPos);
        data.eventCallback(event);
    });
}

void DestroyWindow(GLFWwindow *window) {
    auto *windowProperties = (WindowStruct *) glfwGetWindowUserPointer(window);
    delete windowProperties;
    glfwTerminate();
}
