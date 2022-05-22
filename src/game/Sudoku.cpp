#include <string>
#include <fstream>
#include "Sudoku.h"

bool Sudoku::removeNumber(int i, int j) {
    if (board_[i][j].isReadOnly)
        return false;

    board_[i][j].value = 0;
    return true;
}


bool Sudoku::setNumber(int i, int j, int num) {
    if (board_[i][j].isReadOnly)
        return false;

    if (repeatedInRowOrColumn(i, j, num) || repeatedIn3x3(i, j, num))
        return false;

    board_[i][j].value = num;
    board_[i][j].tile->numericalValue = num;
    return true;
}


bool Sudoku::repeatedIn3x3(int row, int col, int num) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board_[i + col - col % 3][j + row - row % 3].value == num)
                return true;
        }
    }
    return false;
}

bool Sudoku::repeatedInRowOrColumn(int row, int col, int num) const {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board_[col][i].value == num || board_[j][row].value == num)
                return true;
        }
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


void Sudoku::setupSudoku(std::vector<std::vector<int>> &sudoku, Board &board) {
    board_.resize(9);
    for (auto &i: board_) {
        i.resize(9);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = sudoku[i][j];
            auto &b = board_[i][j];
            glm::vec3 pos{j * board.offset_, -i * board.offset_, 0};
            b.tile = &board.addTile(pos, number);
            b.tile->col = i;
            b.tile->row = j;
            b.value = number;
            b.isReadOnly = number != 0;
        }
    }
    board.setupBottomDrawer();
}

Sudoku::Sudoku() {

}
