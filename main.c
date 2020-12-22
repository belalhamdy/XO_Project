#include <stdio.h>

#define GRID_SIZE 3
#define EMPTY -1
#define X 1
#define O 0

/*
 Returns:
 * 0 -> no one wins and TIE
 * +ve number <= GRID_SIZE(3) -> index of row that wins
 * -ve number <= GRID_SIZE(3) -> index of col that wins
 * GRID_SIZE + 1 (4) -> diagonal that goes from left to right wins
 * - (GRID_SIZE + 1) (-4) -> diagonal that goes from right to left wins
 * Anything Else (GRID_SIZE * 4)
 */
int check_win(int grid[GRID_SIZE][GRID_SIZE]);

int computer_turn(int grid[GRID_SIZE][GRID_SIZE], int computer_mark);

void print_grid(int grid[GRID_SIZE][GRID_SIZE]) {
    int i, j;
    for (i = 0; i < GRID_SIZE; ++i) {
        for (j = 0; j < GRID_SIZE; ++j) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int grid[GRID_SIZE][GRID_SIZE] = {
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1}};
    while (check_win(grid) == GRID_SIZE * 4) {
        computer_turn(grid, X);
        print_grid(grid);
        puts("");
        if (check_win(grid) != GRID_SIZE * 4) {
            printf("X WINS\n");
        }
        computer_turn(grid, O);
        print_grid(grid);
        puts("");
        if (check_win(grid) != GRID_SIZE * 4)
            printf("O WINS\n");
        if (!check_win(grid)) return 0;
    }
    return 0;
}

/*
0,0  0,1  0,2
1,0  1,1  1,2
2,0  2,1  2,2
 */
int check_win(int grid[GRID_SIZE][GRID_SIZE]) {
    int i, j;
    int row_wins, col_wins, left_diagonal_wins = 1, right_diagonal_wins = 1;
    for (i = 0; i < GRID_SIZE; ++i) {
        row_wins = (grid[i][0] != EMPTY);
        col_wins = (grid[0][i] != EMPTY);
        for (j = 1; j < GRID_SIZE; ++j) {
            row_wins &= (grid[i][j - 1] == grid[i][j]);
            col_wins &= (grid[j - 1][i] == grid[j][i]);

            if (i == j) {
                left_diagonal_wins &= (grid[0][0] == grid[i][j] && grid[0][0] != -1);
                right_diagonal_wins &= (grid[0][GRID_SIZE - 1] == grid[i][GRID_SIZE - j - 1] &&
                                        grid[0][GRID_SIZE - 1] != -1);
            }
        }
        if (row_wins) return i + 1;
        else if (col_wins) return -(i + 1);
    }
    if (left_diagonal_wins) return GRID_SIZE + 1;
    else if (right_diagonal_wins) return -(GRID_SIZE + 1);

    int empty_cells = 0;
    for (i = 0; i < GRID_SIZE; ++i)
        for (j = 0; j < GRID_SIZE; ++j)
            empty_cells += (grid[i][j] == EMPTY);

    if (empty_cells == 0) return 0;

    return GRID_SIZE * 4; // A unique number that defines that the game has not ended yet
}

/*
 * First try to find a row or column that contains 2 of `computer_mark` and there is an empty cell so you can win
 * If not found try to find a row or column that contains 2 of `opponent_mark` and there is an empty cell so you may lose
 */
int computer_turn(int grid[GRID_SIZE][GRID_SIZE], int computer_mark) {
    const int row = 0, column = 1, diagonal = 2; // First Dimension Size (types)
    const int number_of_my_marks = 0, number_of_empty_cells = 1; // Third Dimension Size
    const int first_dimension_size = 3, second_dimension_size = GRID_SIZE, third_dimension_size = 2;
    int grid_analysis[first_dimension_size][second_dimension_size][third_dimension_size];

    // initialize the grid
    int i, j, k;
    for (i = 0; i < first_dimension_size; ++i)
        for (j = 0; j < second_dimension_size; ++j)
            for (k = 0; k < third_dimension_size; ++k)
                grid_analysis[i][j][k] = 0;

    // study the grid
    for (i = 0; i < GRID_SIZE; ++i) {
        for (j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == computer_mark) grid_analysis[row][i][number_of_my_marks]++;
            else if (grid[i][j] == EMPTY) grid_analysis[row][i][number_of_empty_cells]++;

            if (grid[j][i] == computer_mark) grid_analysis[column][i][number_of_my_marks]++;
            else if (grid[j][i] == EMPTY) grid_analysis[column][i][number_of_empty_cells]++;

            if (i == j) {
                if (grid[i][j] == computer_mark)
                    grid_analysis[diagonal][0][number_of_my_marks]++; // 0 means the left to right diagonal
                else if (grid[i][j] == EMPTY) grid_analysis[diagonal][0][number_of_empty_cells]++;

                int rj = GRID_SIZE - i - 1;
                if (grid[i][rj] == computer_mark)
                    grid_analysis[diagonal][1][number_of_my_marks]++; // 1 means the right to left diagonal
                else if (grid[i][rj] == EMPTY) grid_analysis[diagonal][1][number_of_empty_cells]++;
            }
        }
    }

    // Find a row or a column or diagonal that contains your mark
    int found_idx = -1, found_type, found_importance = 99999, current_importance;
    for (i = 0; i < second_dimension_size; ++i) {
        // Find a way to win
        current_importance = 1;
        if (found_importance > current_importance) {
            if (grid_analysis[row][i][number_of_my_marks] == GRID_SIZE - 1 &&
                grid_analysis[row][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = row;
                found_importance = current_importance;
            } else if (grid_analysis[column][i][number_of_my_marks] == GRID_SIZE - 1 &&
                       grid_analysis[column][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = column;
                found_importance = current_importance;
            } else if (grid_analysis[diagonal][i][number_of_my_marks] == GRID_SIZE - 1 &&
                       grid_analysis[diagonal][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = diagonal;
                found_importance = current_importance;
            }
        }

        // Find a way to prevent your loss
        current_importance = 2;
        if (found_importance > current_importance) {
            if (grid_analysis[row][i][number_of_my_marks] == 0 && grid_analysis[row][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = row;
                found_importance = current_importance;
            } else if (grid_analysis[column][i][number_of_my_marks] == 0 &&
                       grid_analysis[column][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = column;
                found_importance = current_importance;
            } else if (grid_analysis[diagonal][i][number_of_my_marks] == 0 &&
                       grid_analysis[diagonal][i][number_of_empty_cells] == 1) {
                found_idx = i;
                found_type = diagonal;
                found_importance = current_importance;
            }
        }
        // choose the row or column that has 0 of opponent mark
        current_importance = 3;
        if (found_importance > current_importance) {
            if (grid_analysis[row][i][number_of_my_marks] + grid_analysis[row][i][number_of_empty_cells] == GRID_SIZE) {
                found_idx = i;
                found_type = row;
                found_importance = current_importance;
            } else if (grid_analysis[column][i][number_of_my_marks] + grid_analysis[column][i][number_of_empty_cells] ==
                       GRID_SIZE) {
                found_idx = i;
                found_type = column;
                found_importance = current_importance;
            } else if (grid_analysis[diagonal][i][number_of_my_marks] +
                       grid_analysis[diagonal][i][number_of_empty_cells] == GRID_SIZE) {
                found_idx = i;
                found_type = diagonal;
                found_importance = current_importance;
            }
        }
        // Find any cell to fill it
        current_importance = 4;
        if (found_importance > current_importance) {
            if (grid[GRID_SIZE / 2][GRID_SIZE / 2] == EMPTY) {
                found_idx = GRID_SIZE / 2;
                found_type = row;
                found_importance = current_importance;
            } else if (grid_analysis[row][i][number_of_empty_cells]) {
                found_idx = i;
                found_type = row;
                found_importance = current_importance;
            }
        }

    }

    if (found_idx == -1) return 0;

    int ti = -1, tj = -1;
    if (found_type == row) {
        ti = found_idx;
        for (i = 0; i < GRID_SIZE; ++i) {
            if (grid[found_idx][i] == EMPTY) tj = i;
        }
    } else if (found_type == column) {
        tj = found_idx;
        for (i = 0; i < GRID_SIZE; ++i) {
            if (grid[i][found_idx] == EMPTY) ti = i;
        }
    } else if (found_type == diagonal) {
        if (found_idx == 0)
            for (i = 0; i < GRID_SIZE; ++i) {
                if (grid[i][i] == EMPTY) ti = tj = i;
            }
        else
            for (i = 0; i < GRID_SIZE; ++i) {
                if (grid[i][GRID_SIZE - i - 1] == EMPTY) {
                    ti = i;
                    tj = GRID_SIZE - i - 1;
                }
            }
    }

    if (ti == -1 || tj == -1) return 0;
    grid[ti][tj] = computer_mark;
    return 1;
}

/*
 1 0 0 0 1
 0 1 0 1 0
 0 0 1 0 0
 0 1 0 1 0
 1 0 0 0 1
 */