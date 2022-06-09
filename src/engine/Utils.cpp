#include "Utils.h"
#include "glbinding/glbinding.h"
#include <glbinding-aux/debug.h>
#include <iostream>
#include <csignal>

GLFWwindow *tfg::Injector::window = nullptr;
struct tfg::Globals tfg::Injector::globals{};
int tfg::Injector::EntitySize = 0;
EventState *tfg::Injector::eventState = nullptr;

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

    if (_severity == "NOTIFICATION" || _severity == "LOW" || _severity == "PERFORMANCE")
        return;

    LOG_ERROR("{} : {} of {} severity, raised from {}: {}",
              id, _type.c_str(), _severity.c_str(), _source.c_str(), msg);
    raise(SIGINT);
    exit(1);
}


void tfg::InitWindow(const char *title, int width, int height, bool fullscreen) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);


    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow *window = glfwCreateWindow(fullscreen ? mode->width : width,
                                          fullscreen ? mode->height : height,
                                          title,
                                          fullscreen ? monitor : nullptr,
                                          nullptr);

    LOG_DEBUG("Window created {}", fmt::ptr(window));
    if (window == nullptr) {
        LOG_CRITICAL("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    glbinding::Binding::initialize(glfwGetProcAddress);

    glbinding::aux::enableGetErrorCallback();
    glbinding::setCallbackMask(glbinding::CallbackMask::None | glbinding::CallbackMask::ParametersAndReturnValue);
    glbinding::setAfterCallback([](const glbinding::FunctionCall &call) {
        std::string s;
        for (unsigned i = 0; i < call.parameters.size(); ++i) {
            s += fmt::format("{}", (void *) (call.parameters[i].get()));
            if (i < call.parameters.size() - 1)
                s += ", ";
        }

        LOG_TRACE("{} ({}) -> {}", call.function->name(), s, (void *) call.returnValue.get());
    });


    auto shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    auto glVersion = glGetString(GL_VERSION);
    LOG_DEBUG("OpenGL {}, GLSL {}", glVersion, shaderVersion);

    glDebugMessageCallback(errorOccurredGL, nullptr);

    // enable Z-test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSwapInterval(1);

    Injector::window = window;
    Injector::eventState = new EventState();
}

void tfg::ConfigureEvents() {
    assert(Injector::window != nullptr);
    assert(Injector::eventState != nullptr);

    auto window = Injector::window;
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        Injector::eventState->windowSizeHandler(width, height);
    });

    glfwSetWindowPosCallback(window, [](GLFWwindow *window, int xpos, int ypos) {
        Injector::eventState->windowPositionHandler(xpos, ypos);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
        GlobalOptions.shouldClose = true;
    });

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureKeyboard)
            return;

        Injector::eventState->keyboardHandler(key, action, mods);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse && !GlobalOptions.isFreeCamera)
            return;

        Injector::eventState->mouseHandler(button, action, mods);
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse && !GlobalOptions.isFreeCamera)
            return;

        Injector::eventState->mouseScrollHandler(xOffset, yOffset);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xPos, double yPos) {
        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse && !GlobalOptions.isFreeCamera)
            return;

        Injector::eventState->mouseMoveHandler(xPos, yPos);
    });

    glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered) {
        Injector::eventState->mouseEnterHandler(entered);
    });
}

void tfg::DestroyWindow() {
    assert(Injector::window != nullptr);
    assert(Injector::eventState != nullptr);
    delete Injector::eventState;
    glfwTerminate();
}

glm::vec3 tfg::screenToWorld(int x, int y) {
    assert(GlobalOptions.camera != nullptr);

    glm::vec<4, int> viewport{0};
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

    glm::vec3 win;
    if (GlobalOptions.isFreeCamera)
        win = {(float) viewport[2] / 2.0f, viewport[3] / 2.0f, 0};
    else
        win = {(float) x, viewport[3] - (float) y, 0};

    glReadPixels((int) win.x, (int) win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

    glm::vec3 unProject = glm::unProject(win, GlobalOptions.camera->getViewMatrix(), GlobalOptions.camera->getProjectionMatrix(), viewport);

//    LOG_DEBUG("WinDow Pos: {} {} {} Unproject {} {} {}", win.x, win.y, win.z, unProject.x, unProject.y, unProject.z);
    return unProject;
}

glm::vec3 tfg::screenToColor(int x, int y) {
    glm::vec<4, int> viewport{0};
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

    glm::vec3 win{(float) x, viewport[3] - (float) y, 0};

    if (GlobalOptions.isFreeCamera) {
        win.x = viewport[2] / 2.0;
        win.y = viewport[3] / 2.0;
    }

    glm::vec3 color;
    glReadPixels((int) win.x, (int) win.y, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(color));

    return color;
}

int tfg::colorToId(glm::vec3 color) {
    if (color == glm::vec3{GlobalOptions.clearColor})
        return -1;

    int id = std::round(color.r * 10) +
             std::round(color.g * 100) +
             std::round(color.b * 1000);
    assert(id < Injector::EntitySize);
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
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(Injector::window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(Injector::window, GLFW_CURSOR, isEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    GlobalOptions.camera->setFreeCamera(isEnabled);
    GlobalOptions.isFreeCamera = isEnabled;
}

int tfg::getEntityId() {
    return Injector::EntitySize++;
}

void tfg::setClearColor(glm::vec4 const &color) {
    glm::vec4 c{};
    glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(c));
    assert(c == color);
    GlobalOptions.clearColor = color;
}

unsigned int tfg::colorToTexture(glm::vec3 color, const int size) {
    // Create id for texture
    unsigned int tex;
    // generate and bind texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // set texture wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // set image data
    auto *data = new unsigned char[3 * size * size * sizeof(unsigned char)];
    for (unsigned int i = 0; i < size * size; i++) {
        data[i * 3] = (unsigned char) (color.x * 255.0f);
        data[i * 3 + 1] = (unsigned char) (color.y * 255.0f);
        data[i * 3 + 2] = (unsigned char) (color.z * 255.0f);
    }

    // set texture data and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free image memory
    delete[] data;
    return tex;
}
