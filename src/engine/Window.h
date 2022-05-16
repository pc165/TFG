#ifndef TFG_WINDOW_H
#define TFG_WINDOW_H

#include "OpenGL.h"
#include "Logger.h"
#include "Event.h"
#include "Camera.h"

struct WindowStruct {
    int width = 0;
    int height = 0;
    bool isFreeCamera = false;
    bool shouldClose = false;
    std::function<void(const Event &event)> eventCallback;
};

GLFWwindow *InitWindow(const char *title, int width = 1080, int height = 720);

void ConfigureEvents(GLFWwindow *window);

void DestroyWindow(GLFWwindow *window);


#endif //TFG_WINDOW_H
