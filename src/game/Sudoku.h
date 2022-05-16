#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Board.h"
#include "Event.h"
#include "Tools.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>


class Sudoku {
public:
    explicit Sudoku();;

    void drawGUI();

    void draw();

    void drawPickObject();

    bool onEvent(const Event &event, double deltatime);

private:
    glm::vec3 screentoWorldPos{};
    glm::vec3 screenColor{};
    int objectId = -1;
    Board board;
    GameGui gui{};
};


#endif //TFG_SUDOKU_H
