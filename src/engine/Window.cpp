#include <imgui.h>
#include "Window.h"
#include "Utils.h"

GLFWwindow *InitWindow(const char *title, int witdh, int heigth) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(witdh, heigth, title, nullptr, nullptr);

    LOG_DEBUG("Window created {}", fmt::ptr(window));
    if (window == nullptr) {
        LOG_CRITICAL("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        LOG_CRITICAL("Failed to initialize GLEW");
        glfwTerminate();
    }
    glDebugMessageCallback(errorOccurredGL, nullptr);

    // enable Z-test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    return window;
}

void ConfigureEvents(GLFWwindow *window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    auto winProp = new WindowStruct{width, height};
    glfwSetWindowUserPointer(window, winProp);

    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;
        WindowResizeEvent event(EventType::WindowResize, width, height);
        data.eventCallbackQueue.push(std::make_unique<WindowResizeEvent>(event));
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event(EventType::WindowClose);
        data.eventCallbackQueue.push(std::make_unique<WindowCloseEvent>(event));
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
        data.eventCallbackQueue.push(std::make_unique<KeyEvent>(event));
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseButtonEvent event(EventType::MouseButton, button, action == GLFW_PRESS ? 0 : 1);
        data.eventCallbackQueue.push(std::make_unique<MouseButtonEvent>(event));
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;


        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseScrollEvent event(EventType::MouseScrolled, (float) xOffset, (float) yOffset);
        data.eventCallbackQueue.push(std::make_unique<MouseScrollEvent>(event));
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xPos, double yPos) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return;

        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        MouseMoveEvent event(EventType::MouseMoved, (float) xPos, (float) yPos);
        data.eventCallbackQueue.push(std::make_unique<MouseMoveEvent>(event));
    });
}

void DestroyWindow(GLFWwindow *window) {
    auto *events = (WindowStruct *) glfwGetWindowUserPointer(window);
    delete events;
    glfwTerminate();
}
