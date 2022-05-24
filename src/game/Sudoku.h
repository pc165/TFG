#pragma once

#include <vector>
#include "Board.h"

class Sudoku {
public:
    Sudoku();

    void setupSudoku(std::vector<std::vector<int>> &, Board &);

    bool removeNumber(int, int);

    bool setNumber(int num, int i, int j);

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] bool repeatedIn3x3(int row, int col, int num) const;

    [[nodiscard]] bool repeatedInRowOrColumn(int row, int col, int num) const;

    struct Cell {
        int value{0};
        bool isReadOnly{false};
        Tile *tile{nullptr};
    };

private:

    std::vector<std::vector<Cell>> board_;
};
