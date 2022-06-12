#pragma once

#include <vector>
#include "render/Board.h"

class Sudoku {
public:
    Sudoku();

    void setupSudoku(std::vector<std::vector<int>> const &, Board &);

    bool loadSudoku(std::string const &path);

    bool setNumber(int row, int col, int j);

    void randomSudokuGenerator(int difficulty = 0);

    void printSudoku();

    bool findUnsetColRow(int &row, int &col) const;

    bool updateSolutions();

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] bool repeatedIn3x3(int row, int col, int num) const;

    [[nodiscard]] bool repeatedInRow(int row, int num) const;

    [[nodiscard]] bool repeatedInColumn(int col, int num) const;

    [[nodiscard]] int getSolution(int row, int col) const;

    [[nodiscard]] int isReadOnly(int row, int col) const;

    [[nodiscard]] bool isSafe(int row, int col, int num) const;

private:
    std::vector<std::vector<tfg::Cell>> board_;
};
