// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "engine/Shader.h"
#include "engine/utils.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/Gui.h"


int main(int argc, char *argv[]) {

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "TFG", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glDebugMessageCallback(errorOccurredGL, nullptr);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Shader shader("shader.glsl");

    GLfloat triangle[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
    };

    GLuint index[] = {
            0, 1, 2,
            2, 3, 0
    };
    {

        VertexBuffer vb(triangle, sizeof(triangle), GL_STATIC_DRAW);
        IndexBuffer<uint32_t> ib(index, 6, GL_STATIC_DRAW);
        ib.bind();
        vb.bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

        GLint uniformId = glGetUniformLocation(shader.getProgramId(), "uColor");

        float g = 0.0f, inc = 0.05f;
        bool show_demo_window = true;
        Gui gui(window);

        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
            glfwPollEvents();

            gui.newFrame();
            ImGui::ShowDemoWindow(&show_demo_window);

            shader.bind();
            glBindVertexArray(VertexArrayID);
            if (uniformId != -1) {
                glUniform4f(uniformId, 0.5f, g, 0.5f, 0.1f);
                if (g > 1.0f)
                    inc = -0.05f;
                else if (g < 0.0f)
                    inc = +0.05f;
                g += inc;
            }

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            gui.render();
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        glDeleteVertexArrays(1, &VertexArrayID);
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
