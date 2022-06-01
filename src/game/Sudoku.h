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

    bool setNumber(int row, int col, int j);

    void restartSudoku(std::vector<std::vector<int>> const &sudoku) {
        assert(!sudoku.empty());
        assert(!sudoku[0].empty());

        for (int row = 0; row < (int) sudoku.size(); row++) {
            for (int col = 0; col < (int) sudoku[row].size(); col++) {
                int number = sudoku[col][row];
                auto &b = board_[row][col];
                b.value = number;
                b.solution = 0;
                b.isReadOnly = number != 0;
                b.tile->numericalValue = number;
                b.tile->isHintsEnabled = true;
            }
        }
        updateSolutions();
    }

    void randomSudokuGenerator(int difficulty = 0) {

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

        for (int i = 0; i < (int) board_.size(); i += 3) {
            int row = i, col = i, num = 0;
            for (int j = row; j < 3 + row; ++j) {
                for (int l = col; l < 3 + col; ++l) {
                    do {
                        num = random() % 10;
                    } while (num == 0 || repeatedIn3x3(j, l, num));
                    board_[j][l].value = num;
                    board_[j][l].solution = num;
                }
            }
        }

        updateSolutions();
        printSudoku();

        for (auto &i: board_) {
            for (auto &j: i) {
                int val = j.solution;
                j.value = val;
                j.tile->numericalValue = val;
                j.isReadOnly = true;
            }
        }

        if (!isDone()){
            LOG_ERROR("SUDOKU CREATION ERROR!!");
            assert(false);
        }

        int d[] = {3, 5, 7};
        for (int i = 0; i < d[difficulty]; i++) {
            int row, col;
            tfg::Cell *b;
            do {
                row = random() % 9;
                col = random() % 9;
                assert(row >= 0 && row < (int) board_.size());
                assert(col >= 0 && col < (int) board_[0].size());
                b = &board_[row][col];
            } while (b->value == 0);
            assert(board_[row][col].solution != 0);
            b->value = 0;
            b->isReadOnly = false;
            b->tile->numericalValue = 0;
            b->tile->cube.color = CUBE_COLOR_DEFAULT;
            b->tile->isHintsEnabled = true;
        }
        printSudoku();
    }

    void printSudoku() {
        std::string s;
        for (auto &i: board_) {
            for (auto &j: i) {
                s += fmt::format("{} ", j.solution);
            }
            s += "\n";
        }
        LOG_INFO("\nNew sudoku\n{}", s);
    }

    bool findUnsetColRow(int &row, int &col) const {
        for (row = 0; row < (int) board_.size(); ++row) {
            for (col = 0; col < (int) board_[row].size(); ++col) {
                if (board_[row][col].value == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    bool updateSolutions() {
        int row, col;
        if (!findUnsetColRow(row, col))
            return true;

        assert(row >= 0 && row < (int) board_.size());
        assert(col >= 0 && col < (int) board_[0].size());

        for (int value = 1; value <= 9; value++) {
            if (isSafe(row, col, value)) {
                auto &cell = board_[row][col];
                assert(!cell.isReadOnly);
                assert(cell.solution == 0);
                cell.value = value;
                if (updateSolutions()) {
                    cell.value = 0;
                    cell.solution = value;
                    return true;
                }
                board_[row][col].value = 0;
            }
        }
        return false;
    }

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] bool repeatedIn3x3(int row, int col, int num) const;

    [[nodiscard]] bool repeatedInRow(int row, int num) const;

    [[nodiscard]] bool repeatedInColumn(int col, int num) const {
        for (int i = 0; i < 9; ++i) {
            if (board_[i][col].value == num)
                return true;
        }
        return false;
    }


    [[nodiscard]] int getSolution(int row, int col) const {
        assert(row >= 0 && row < (int) board_.size());
        assert(col >= 0 && col < (int) board_[0].size());
        return board_[row][col].solution;
    }

    [[nodiscard]] int isReadOnly(int row, int col) const {
        assert(row >= 0 && row < (int) board_.size());
        assert(col >= 0 && col < (int) board_[0].size());
        return board_[row][col].isReadOnly;
    }


    [[nodiscard]]  bool isSafe(int row, int col, int num) const {
        return !repeatedIn3x3(row, col, num) && !repeatedInRow(row, num) && !repeatedInColumn(col, num);
    }

private:
    std::vector<std::vector<tfg::Cell>> board_;
};
