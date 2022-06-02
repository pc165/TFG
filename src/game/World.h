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

    void gameLoop();

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


    std::vector<std::vector<int>> sudokuValues_{};

    std::string sudokuPath_{};
    float frametime_{0};
    int difficulty{0};
    bool lockLightPosition{true};
    float cutoffDegress{12.0f};
    float outcutoffDegress{90.0f};
    tfg::Tile *hoveredTile_{nullptr};
    tfg::Tile *selectedTile_{nullptr};
    tfg::Tile *nearesTile_{nullptr};
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
