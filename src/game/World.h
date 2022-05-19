#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "GameGui.h"
#include "Board.h"
#include "Event.h"
#include "Tools.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>


class World {
public:
    explicit World() {
        Tools::camera = &this->camera_;
        assert(Tools::camera != nullptr);
        assert(Tools::window != nullptr);
        assert(Tools::windowStruct != nullptr);
        int number = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                board_.addTile({j, i, 0}, number % 10);
                number++;
            }
        }
    }

    void gameLoop() {
        auto windowProps = Tools::windowStruct;
        windowProps->eventCallback = [this](const Event &event) {
            this->onEvent(event, this->deltaFrmame_);
        };

        double t0 = 0;
        while (!windowProps->shouldClose) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            board_.drawPickObject();
            glfwPollEvents();
            glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto t1 = glfwGetTime();
            deltaFrmame_ = t1 - t0;
            gui_.drawwGUI();
            board_.draw();
            t0 = t1;
            glfwSwapBuffers(Tools::window);
        };
    };

    void onEvent(const Event &event, double deltatime) {
        camera_.onEvent(event, deltaFrmame_);
        gui_.onEvent(event, deltaFrmame_);
        switch (event.type) {
            default:
                break;
        }
    }


private:
    Camera camera_{{4, 4, 12}};
    GameGui gui_{};
    Board board_{};
    float deltaFrmame_{0};
};


#endif //TFG_WORLD_H
