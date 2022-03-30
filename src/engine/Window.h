#ifndef TFG_WINDOW_H
#define TFG_WINDOW_H

#include "OpenGL.h"
#include "Utils.h"
#include "Logger.h"

GLFWwindow *InitWindow(const char *title, int witdh = 1080, int heigth = 720);

void ConfigureEvents(GLFWwindow *window);

void DestroyWindow(GLFWwindow *window);


#endif //TFG_WINDOW_H
