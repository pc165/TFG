#include "Sudoku.h"
#include "Constants.h"

bool Sudoku::setNumber(int row, int col, int num) {
    assert(col >= 0 && col < (int) board_.size());
    assert(row >= 0 && row < (int) board_[0].size());

    if (board_[row][col].isReadOnly)
        return false;

    if (num != 0 && !isSafe(row, col, num))
        return false;

    board_[row][col].value = num;
    board_[row][col].tile->numericalValue = num;
    return true;
}


bool Sudoku::repeatedIn3x3(int row, int col, int num) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board_[i + row - row % 3][j + col - col % 3].value == num)
                return true;
        }
    }
    return false;
}

bool Sudoku::repeatedInRow(int row, int num) const {
    for (int j = 0; j < 9; ++j) {
        if (board_[row][j].value == num)
            return true;
    }
    return false;
}

bool Sudoku::isDone() const {
    for (auto &i: board_) {
        for (auto &j: i) {
            if (j.value == 0)
                return false;
        }
    }
    return true;
}


void Sudoku::setupSudoku(std::vector<std::vector<int>> const &sudoku, Board &board) {
    assert(!sudoku.empty());
    assert(!sudoku[0].empty());

    board_.resize(9);
    for (auto &i: board_) {
        i.resize(9);
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int number = sudoku[col][row];
            auto &b = board_[row][col];

            glm::vec3 pos{row * board.offset_, -col * board.offset_, 0};
            auto tile = board.getTile(board.addTile(pos, number));
            assert(tile != nullptr);
            b.tile = tile;
            b.tile->row = row;
            b.tile->col = col;
            b.value = number;
            b.tile->cube.color = number == 0 ? CUBE_COLOR_DEFAULT : b.tile->cube.color;
            b.isReadOnly = number != 0;
        }
    }
    updateSolutions();
    board.setupDeck();
}

Sudoku::Sudoku() {

}
