#include <algorithm>
#include "App.h"
#include "utils.h"


App *App::instance_ = nullptr;

App::App(const std::string &title, uint32_t witdh, uint32_t heigth) : window_(nullptr),
                                                                      layers_(),
                                                                      lastFrameTime(0.0),
                                                                      shouldClose(false),
                                                                      guiLayer() {
    instance_ = this;
    LOG_INFO("App created {}.", fmt::ptr(instance_));
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG_CRITICAL("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    window_ = glfwCreateWindow((int32_t) witdh, (int32_t) heigth, title.c_str(), nullptr, nullptr);

    LOG_DEBUG("Window created {}", fmt::ptr(window_));

    if (window_ == nullptr) {
        LOG_CRITICAL("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(window_);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        LOG_CRITICAL("Failed to initialize GLEW");
        glfwTerminate();
    }
    glDebugMessageCallback(errorOccurredGL, nullptr);
}

App::~App() {
    glfwTerminate();
}

void App::removeLayer(const Layer *layer) {
    auto it = std::find(layers_.begin(), layers_.end(), layer);
    if (it != layers_.end()) {
        (*it)->onDetach();
        layers_.erase(it);
        LOG_INFO("Layer {} removed", layer->getName());
    } else {
        spdlog::error("Layer {} not found", layer->getName());
    }
}

void App::pushLayer(Layer *layer) {
    layer->onAttach();
    layers_.push_back(layer);
}

void App::run() {
    guiLayer.onAttach();
    while (!shouldClose) {
        for (auto &layer: layers_) {
            double timeSeconds = glfwGetTime();
            double deltaTime = timeSeconds - lastFrameTime;
            lastFrameTime = timeSeconds;
            (*layer).onUpdate(deltaTime);
        }

        // TODO Call glDraw

        guiLayer.begin();
        for (auto &layer: layers_) {
            (*layer).onGuiRender();
        }
        guiLayer.end();

        glfwPollEvents();
        glfwSwapBuffers(window_);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    guiLayer.onDetach();
}

bool App::isShouldClose() const {
    return shouldClose;
}

void App::setShouldClose(bool close) {
    App::shouldClose = close;
}
