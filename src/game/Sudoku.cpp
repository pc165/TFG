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


Sudoku::Sudoku(const std::string &name) {
    std::ifstream file;
    file.open(name);
    board_.resize(9);
    for (auto &i: board_) {
        i.resize(9);
    }
    if (!file.is_open())
        return;

    for (auto &i: board_) {
        for (auto &j: i) {
            int number = 0;
            file >> number;
            j.value = number;
            if (number != 0)
                j.isReadOnly = true;
        }
    }
}

Sudoku::Sudoku() {
    board_.resize(9);
    for (auto &i: board_) {
        i.resize(9);
    }
}
