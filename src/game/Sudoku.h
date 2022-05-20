#pragma once

#include <iostream>
#include <vector>

class Sudoku {
public:
    Sudoku();

    explicit Sudoku(const std::string &);

    bool removeNumber(int, int);

    bool setNumber(int num, int i, int j);

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] bool repeatedIn3x3(int row, int col, int num) const;

    [[nodiscard]] bool repeatedInRowOrColumn(int row, int col, int num) const;

    struct Cell {
        int value = 0;
        bool isReadOnly = false;
    };

private:

    std::vector<std::vector<Cell>> board_;
};
