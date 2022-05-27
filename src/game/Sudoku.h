#pragma once

#include <vector>
#include "render/Board.h"

class Sudoku {
public:
    Sudoku();

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

    static void randomSudokuGenerator(std::vector<std::vector<int>> &array) {
        array.clear();
        array.resize(9);
        for (auto &i: array) {
            i.resize(9);
        }
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
                assert(cell.isReadOnly == 0);
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
