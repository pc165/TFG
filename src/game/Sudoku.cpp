#include "Sudoku.h"
#include "Constants.h"

bool Sudoku::setNumber(int row, int col, int num) {
    assert(col >= 0 && col < (int) board_.size());
    assert(row >= 0 && row < (int) board_[0].size());

    if (board_[row][col].isReadOnly)
        return false;

    if (!isSafe(row, col, num))
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
    int sum = 0;
    for (auto &i: board_) {
        for (auto &j: i) {
            sum += j.value;
        }
    }

    if (sum != 405) // sum of a 9 x 9 sudoku (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9)* 9= 117
        return false;
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
            b.tile->col = col;
            b.value = number;
            b.tile->cube.color = number == 0 ? CUBE_COLOR_DEFAULT : CUBE_COLOR_ASSIGNED;
            b.tile->isHintsEnabled = number == 0;
            b.isReadOnly = number != 0;
        }
    }
    updateSolutions();
    board.setupDeck();
}

void Sudoku::randomSudokuGenerator(int difficulty) {

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
                    num = rand() % 10;
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

    if (!isDone()) {
        LOG_ERROR("SUDOKU CREATION ERROR!!");
        assert(false);
    }

    int d[] = {3, 5, 7};
    for (int i = 0; i < d[difficulty]; i++) {
        int row, col;
        tfg::Cell *b;
        do {
            row = rand() % 9;
            col = rand() % 9;
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

void Sudoku::printSudoku() {
    std::string s;
    for (auto &i: board_) {
        for (auto &j: i) {
            s += fmt::format("{} ", j.solution);
        }
        s += "\n";
    }
    LOG_INFO("\nNew sudoku\n{}", s);
}

bool Sudoku::findUnsetColRow(int &row, int &col) const {
    for (row = 0; row < (int) board_.size(); ++row) {
        for (col = 0; col < (int) board_[row].size(); ++col) {
            if (board_[row][col].value == 0) {
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::updateSolutions() {
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

bool Sudoku::repeatedInColumn(int col, int num) const {
    for (int i = 0; i < 9; ++i) {
        if (board_[i][col].value == num)
            return true;
    }
    return false;
}

int Sudoku::getSolution(int row, int col) const {
    assert(row >= 0 && row < (int) board_.size());
    assert(col >= 0 && col < (int) board_[0].size());
    return board_[row][col].solution;
}

bool Sudoku::isSafe(int row, int col, int num) const {
    return !repeatedIn3x3(row, col, num) && !repeatedInRow(row, num) && !repeatedInColumn(col, num);
}

int Sudoku::isReadOnly(int row, int col) const {
    assert(row >= 0 && row < (int) board_.size());
    assert(col >= 0 && col < (int) board_[0].size());
    return board_[row][col].isReadOnly;
}
