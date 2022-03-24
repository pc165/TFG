#include <memory>
#include "OpenGL.h"
#include "Logger.h"
#include "utils.h"
#include "Sudoku.h"

void initLogger() {
    spdlog::set_pattern("[%H:%M:%S.%e] [%l] [%s:%#:%!] %v");
    spdlog::set_level(spdlog::level::trace);
}

GLFWwindow *initWindow(const char *title, int witdh = 1080, int heigth = 720) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(witdh, heigth, title, nullptr, nullptr);

    auto winProp = new EventStruct{witdh, heigth};
    glfwSetWindowUserPointer(window, winProp);

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

void configureEvents(GLFWwindow *window) {
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *events = reinterpret_cast<EventStruct *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE) {
            events->shouldClose = true;
        }
    });
}

void destroyWindow(GLFWwindow *window) {
    auto *events = (EventStruct *) glfwGetWindowUserPointer(window);
    delete events;
    glfwTerminate();
}

int main(int, char *[]) {
    initLogger();
    auto window = initWindow("TFG");
    auto events = (EventStruct *) glfwGetWindowUserPointer(window);
    configureEvents(window);
    double lastFrameTime = 0;
    {
        Sudoku game(window);
        glClearColor(0, 0, 0.5, 0);
        do {
            double timeSeconds = glfwGetTime();
            double deltaTime = timeSeconds - lastFrameTime;
            lastFrameTime = timeSeconds;
            game.render(deltaTime);
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();
        } while (!events->shouldClose);
    }

    destroyWindow(window);
    return 0;
}
