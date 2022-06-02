#pragma once

#include <vector>
#include <random>
#include "render/Board.h"

class Sudoku {
public:
    Sudoku() {
        srandom(0);
    }

    void setupSudoku(std::vector<std::vector<int>> const &, Board &);

    bool loadSudoku(std::string const &path) {
        std::ifstream file;
        file.open(path);


        if (file.is_open()) {
            // reset board
            for (auto &i: board_) {
                for (auto &j: i) {
                    j.value = 0;
                    j.solution = 0;
                    j.isReadOnly = false;
                    j.tile->hints = 0;
                    j.tile->isHintsEnabled = false;
                    j.tile->cube.color = CUBE_COLOR_ASSIGNED;
                }
            }

            for (int i = 0; i < 9 && !file.eof(); ++i) {
                for (int j = 0; j < 9 && !file.eof(); ++j) {
                    int number = 0;
                    file >> number;
                    auto &b = board_[i][j];
                    b.value = number;
                    b.tile->numericalValue = number;
                    b.tile->cube.color = number == 0 ? CUBE_COLOR_DEFAULT : CUBE_COLOR_ASSIGNED;
                    b.tile->isHintsEnabled = number == 0;
                    b.isReadOnly = number != 0;
                }
            }
            updateSolutions();
        } else {
            LOG_WARN("Cannot open {}",path);
            return false;
        }
        return true;
    }

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
