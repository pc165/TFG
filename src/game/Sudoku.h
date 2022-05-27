#pragma once

#include <vector>
#include "render/Board.h"

class Sudoku {
public:
    Sudoku();

    void setupSudoku(std::vector<std::vector<int>> const &, Board &);

    bool setNumber(int row, int col, int j);

    void restartSudoku(std::vector<std::vector<int>> const &sudoku) {
        for (int col = 0; col < 9; col++) {
            for (int row = 0; row < 9; row++) {
                int number = sudoku[col][row];
                auto &b = board_[col][row];
                b.value = number;
                b.tile->numericalValue = number;
                b.isReadOnly = number != 0;
            }
        }
    }

    void randomSudokuGenerator() {
        std::vector<std::vector<int>> s{};
        s.resize(9);
        for (auto &i: s) {
            i.resize(9);
        }
    }

    bool findUnsetColRow(int &row, int &col) {
        for (size_t i = 0; i < board_.size(); ++i) {
            for (size_t j = 0; j < board_[i].size(); ++j) {
                if (board_[i][j].value == 0) {
                    row = i;
                    col = j;
                    return true;
                }
            }
        }
        return false;
    }

//    bool fillRemaining(int i, int j) {
//        //  System.out.println(i+" "+j);
//        const int SRN = 9;
//        const int N = board_.size();
//        if (j >= N && i < N - 1) {
//            i = i + 1;
//            j = 0;
//        }
//        if (i >= N && j >= N)
//            return true;
//
//        if (i < SRN) {
//            if (j < SRN)
//                j = SRN;
//        } else if (i < N - SRN) {
//            if (j == (int) (i / SRN) * SRN)
//                j = j + SRN;
//        } else {
//            if (j == N - SRN) {
//                i = i + 1;
//                j = 0;
//                if (i >= N)
//                    return true;
//            }
//        }
//
//        for (int num = 1; num <= N; num++) {
//            if (isSafe(i, j, num)) {
//                mat[i][j] = num;
//                if (fillRemaining(i, j + 1))
//                    return true;
//
//                mat[i][j] = 0;
//            }
//        }
//        return false;
//    }

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] bool repeatedIn3x3(int row, int col, int num) const;

    [[nodiscard]] bool repeatedInRowOrColumn(int row, int col, int num) const;

    bool isSafe(int row, int col, int num) {
        return !repeatedIn3x3(row, col, num) && !repeatedInRowOrColumn(row, col, num);
    }

    struct Cell {
        int value{0};
        bool isReadOnly{false};
        Tile *tile{nullptr};
    };

private:

    std::vector<std::vector<Cell>> board_;
};
