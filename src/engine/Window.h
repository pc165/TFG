#ifndef TFG_WINDOW_H
#define TFG_WINDOW_H

#include "OpenGL.h"
#include "Logger.h"
#include "Event.h"
#include <memory>
#include <queue>

struct WindowStruct {
    int width = 0;
    int height = 0;
    bool shouldClose = false;
    std::queue<std::unique_ptr<Event>> eventCallbackQueue{};
};

GLFWwindow *InitWindow(const char *title, int witdh = 1080, int heigth = 720);

void ConfigureEvents(GLFWwindow *window);

void DestroyWindow(GLFWwindow *window);


#endif //TFG_WINDOW_H
