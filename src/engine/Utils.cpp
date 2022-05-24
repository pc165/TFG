#include "Utils.h"

GLFWwindow *tfg::Injector::window = nullptr;
tfg::WindowStruct *tfg::Injector::windowStruct = nullptr;
Camera *tfg::Injector::camera = nullptr;
int tfg::Injector::EntitySize = 0;

void tfg::InitLogger() {
    spdlog::set_pattern("[%H:%M:%S.%e] [%l] [%s:%#:%!] %v");
    spdlog::set_level(spdlog::level::trace);
}

void tfg::glfw_error_callback(int error, const char *description) {
    LOG_ERROR("Glfw Error {}: {}", error, description);
}

void tfg::errorOccurredGL(GLenum source,
                          GLenum type,
                          GLuint id,
                          GLenum severity,
                          GLsizei length,
                          const GLchar *msg,
                          const void *userParam) {
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;
        default:
            _source = "UNKNOWN";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;
        default:
            _type = "UNKNOWN";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;
        default:
            _severity = "UNKNOWN";
            break;
    }

    if (_severity == "NOTIFICATION" || _severity == "LOW")
        return;
    LOG_ERROR("{} : {} of {} severity, raised from {}: {}",
              id, _type.c_str(), _severity.c_str(), _source.c_str(), msg);
    //raise(SIGTRAP);
    exit(1);
}


GLFWwindow *tfg::InitWindow(const char *title, int width, int height) {
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
    glfwSwapInterval(1);
    auto winProp = new WindowStruct{width, height};
    glfwSetWindowUserPointer(window, winProp);

    Injector::window = window;
    Injector::windowStruct = winProp;
    return window;
}

void tfg::ConfigureEvents(GLFWwindow *window) {
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
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        if (io.WantCaptureMouse && !data.isFreeCamera)
            return;

        MouseButtonEvent event(EventType::MouseButton, button, action == GLFW_PRESS ? 0 : 1);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        ImGuiIO &io = ImGui::GetIO();
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        if (io.WantCaptureMouse && !data.isFreeCamera)
            return;

        MouseScrollEvent event(EventType::MouseScrolled, (float) xOffset, (float) yOffset);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xPos, double yPos) {
        ImGuiIO &io = ImGui::GetIO();
        WindowStruct &data = *reinterpret_cast<WindowStruct *>(glfwGetWindowUserPointer(window));
        if (io.WantCaptureMouse && !data.isFreeCamera)
            return;

        MouseMoveEvent event(EventType::MouseMoved, (float) xPos, (float) yPos);
        data.eventCallback(event);
    });
}

void tfg::DestroyWindow(GLFWwindow *window) {
    auto *windowProperties = (WindowStruct *) glfwGetWindowUserPointer(window);
    delete windowProperties;
    glfwTerminate();
}

glm::vec3 tfg::screenToWorld(int x, int y, const glm::vec3 &point) {
    assert(Injector::camera != nullptr);
    glm::vec<4, int> viewport{0};
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

    glm::vec3 win;
    if (Injector::windowStruct->isFreeCamera)
        win = {(float) viewport[2] / 2.0f, viewport[3] / 2.0f, 0};
    else
        win = {(float) x, viewport[3] - (float) y, 0};

    glReadPixels((int) win.x, (int) win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

    glm::vec3 unProject = glm::unProject(win, Injector::camera->getViewMatrix(), Injector::camera->getProjectionMatrix(), viewport);

    LOG_DEBUG("WinDow Pos: {} {} {} Unproject {} {} {}", win.x, win.y, win.z, unProject.x, unProject.y, unProject.z);
    return unProject;
}

glm::vec3 tfg::screenToColor(int x, int y) {
    glm::vec<4, int> viewport{0};
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

    glm::vec3 win{(float) x, viewport[3] - (float) y, 0};

    if (Injector::windowStruct->isFreeCamera) {
        win.x = viewport[2] / 2.0;
        win.y = viewport[3] / 2.0;
    }

    if (win.x < viewport[0] || win.x > viewport[2] ||
        win.y < viewport[1] || win.y > viewport[3]) {
        glm::vec4 clearColor;
        LOG_TRACE("Mouse outside viewport");
        glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clearColor));
        return glm::vec3{clearColor};
    }

    glm::vec3 color;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels((int) win.x, (int) win.y, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(color));

    return color;
}

int tfg::colorToId(glm::vec3 color) {
    glm::vec4 clearColor;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clearColor));

    if (color == glm::vec3{clearColor})
        return -1;

    int id = std::round(color.r * 10) +
             std::round(color.g * 100) +
             std::round(color.b * 1000);

    return id;
}

glm::vec3 tfg::genPickColor(int n) {
    float r = int((n % 10)) / 10.f;
    float g = int((n % 100) / 10) / 10.f;
    float b = int((n % 1000) / 100) / 100.f;
    glm::vec3 newColor(r, g, b);

    LOG_DEBUG("Pick color {}", glm::to_string(newColor).c_str());
    return newColor;
}

void tfg::setFreeCamera(bool isEnabled) {
    if (isEnabled) {
        glfwSetInputMode(Injector::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(Injector::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    glfwSetCursorPos(Injector::window, Injector::windowStruct->width / 2.0, Injector::windowStruct->height / 2.0);
    Injector::camera->setFirstMove();
    Injector::camera->setFreeCamera(isEnabled);
    Injector::windowStruct->isFreeCamera = isEnabled;
}

int tfg::getEntityId() {
    return Injector::EntitySize++;
}
