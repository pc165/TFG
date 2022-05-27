#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "render/GameGui.h"
#include "render/Board.h"
#include "Sudoku.h"
#include "render/Crosshair.h"
#include "Utils.h"


class World {
public:
    explicit World();

    void gameLoop();;

    void guiOverlay() const;

    void guiWindow();

    bool onUpdate(float deltatme);

private:
    // game UI
    GameGui gui_{};

    // Sudoku tiles controller
    Board board_{};

    Camera camera_{Injector::eventState, {4 * board_.offset_, -4 * board_.offset_, 35}};

    // 2D crosshair render
    Crosshair crosshairRender_{};

    // Sudoku logic
    Sudoku sudoku_{};

    // Event state machine
    EventState *eventState_{Injector::eventState};

    float frametime_{0};
    Tile *hoveredTile_{nullptr};
    Tile *selectedTile_{nullptr};
    Tile *nearesTile_{nullptr};
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
