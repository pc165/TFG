#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Cube.h"
#include "Event.h"
#include "EventState.h"
#include <glm/gtx/string_cast.hpp>


class Sudoku {
public:
    explicit Sudoku() : camera({0, 0, 5}), c(&camera) {
        glm::vec3 scale(0.1);
        c.add({0, 0, 0}, scale);
        c.add({0, 1, 0}, scale);
//        for (int i = 0; i < 9; i++) {
//            for (int j = 0; j < 3; j++) {
//                for (int z = 0; z < 3; z++) {
//                    glm::vec3 pos(j + i, z + i, 0);
//                    c.add(pos, scale);
//                }
//            }
//        }
    };

    void render(double deltaSeconds) {
        c.draw();
        GameGui::begin();
        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", (float *) &camera.pos, -10, 10);
        ImGui::SliderFloat3("Center", (float *) &camera.center, -10, 10);
        ImGui::SliderFloat3("Up", (float *) &camera.up, -1, 1);
        ImGui::End();

        ImGui::Begin("Model");
        ImGui::SliderInt("Cube id", &selector, 0, c.size - 1);
        ImGui::SliderFloat3("Position", (float *) &c.position_[selector], -10, 10);
        ImGui::SliderFloat3("Scale", (float *) &c.scale_[selector], 1, 10);
        ImGui::SliderFloat3("Axis", (float *) &c.rotationAxis_[selector], -1, 1);
        ImGui::SliderFloat("Degrees", &c.degrees_[selector], -180, 180);
        ImGui::End();

        GameGui::showOverlay(nullptr, [this]() {
            ImGui::Text("S2W: (%s)", glm::to_string(this->screentoWorldPos).c_str());
            ImGui::Text("S2C: (%s)", glm::to_string(this->screentoWorldColor).c_str());
        });
        GameGui::end();
    }

    glm::vec3 screenToWorld(int x, int y) {
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};
        glReadPixels(x, int(win.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);
        return glm::unProject(win, camera.getViewMatrix(), camera.getProjectionMatrix(), viewport);
    }


    glm::vec3 screenToColor(int x, int y) {
        glm::vec3 color;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(color));
        int id = round(color.r * 10) +
                 round(color.g * 100) +
                 round(color.b * 1000);
        return color;
    }

    bool onEvent(const Event &event, double deltatime) {
        camera.onEvent(event, deltatime);
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                screentoWorldPos = screenToWorld(mouse->xPos, mouse->yPos);
                screentoWorldColor = screenToColor(mouse->xPos, mouse->yPos);
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                break;
            }
            default:
                break;
        }
        return false;
    }

private:
    glm::vec3 screentoWorldPos{};
    glm::vec3 screentoWorldColor{};
    int selector = 0;
    Camera camera;
    Cube c;
    GameGui gui{};
};


#endif //TFG_SUDOKU_H
