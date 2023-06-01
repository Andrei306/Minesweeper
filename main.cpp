#include <iostream>
#include <vector>
#include <random>

/**
 * @brief This is the basic class for our Minesweeper game.
 *
 * The Minesweeper class represents a Minesweeper game with a rectangular game board
 * consisting of cells. The game board contains hidden mines that the player must avoid
 * revealing. The goal of the game is to reveal all non-mine cells without triggering
 * a mine. The class provides methods to interact with the game, such as revealing cells,
 * checking the game state, and printing the game board.
 */

class Minesweeper
{
private:
    int rows, cols, num_mines;
    std::vector<std::vector<bool>> minefield;
    std::vector<std::vector<bool>> revealed;
    std::vector<std::vector<int>> counts;

public:
    /**
     * @brief Constructs a Minesweeper game with the specified dimensions and number of mines.
     *
     * @param rows_ The number of rows in the game board.
     * @param cols_ The number of columns in the game board.
     * @param num_mines_ The number of mines to be placed on the game board.
     *
     * This constructor initializes the game board, randomly places the specified number of mines,
     * and calculates the mine counts for each square. The minefield, revealed, and counts vectors
     * are initialized accordingly.
     */

    Minesweeper(int rows_, int cols_, int num_mines_) : rows(rows_), cols(cols_), num_mines(num_mines_)
    {
        // Initialize the minefield and revealed vectors
        minefield = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
        revealed = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));

        // Initialize the counts vector to all zeros
        counts = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));

        // Randomly place the mines
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> row_dist(0, rows - 1);
        std::uniform_int_distribution<> col_dist(0, cols - 1);

        int num_placed = 0;

        while (num_placed < num_mines)
        {
            int row = row_dist(gen);
            int col = col_dist(gen);
            if (!minefield[row][col])
            {
                minefield[row][col] = true;
                num_placed++;
            }
        }

        // Calculate the counts for each square
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (minefield[row][col])
                {
                    counts[row][col] = -1; // This square is a mine
                }
                else
                {
                    int count = 0;
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (row + i >= 0 && row + i < rows &&
                                    col + j >= 0 && col + j < cols &&
                                    minefield[row + i][col + j])
                            {
                                count++;
                            }
                        }
                    }
                    counts[row][col] = count;
                }
            }
        }
    }

    /*void print() const {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (revealed[row][col]) {
                    if (counts[row][col] == -1) {
                        std::cout << "* ";
                    } else {
                        std::cout << counts[row][col] << " ";
                    }
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }*/

    /**
     * @brief Prints the current state of the Minesweeper game board.
     *
     * This function prints the game board to the console, showing the revealed cells
     * and their corresponding values or mine indicators. Each cell is represented by a
     * character: '*' for a revealed mine, a number for a revealed non-mine cell indicating
     * the number of adjacent mines, and '.' for a hidden cell. The row and column numbers
     * are displayed at the top and left side of the board, respectively, to assist with cell
     * identification.
     *
     * Note: Only the revealed cells are printed, and hidden cells are represented by '.'.
     */

    void print() const
    {
        // Print column numbers
        std::cout << "  ";
        for (int col = 0; col < cols; col++)
        {
            std::cout << col << " ";
        }
        std::cout << std::endl;

        // Print board with row numbers
        for (int row = 0; row < rows; row++)
        {
            std::cout << row << " ";
            for (int col = 0; col < cols; col++)
            {
                if (revealed[row][col])
                {
                    if (counts[row][col] == -1)
                    {
                        std::cout << "* ";
                    }
                    else
                    {
                        std::cout << counts[row][col] << " ";
                    }
                }
                else
                {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief Checks if the game is over.
     *
     * This function determines whether the game is over by checking if any mines have been revealed.
     * It iterates through each cell on the game board and checks if it contains a mine and has been revealed.
     * If a mine is found that has been revealed, it indicates that the game is over.
     *
     * @return true if a mine has been revealed and the game is over.
     * @return false if no mine has been revealed and the game is still ongoing.
     */

    bool is_game_over() const
    {
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (minefield[row][col] && revealed[row][col])
                {
                    return true; // A mine was revealed
                }
            }
        }
        return false;
    }

    /**
     * @brief Checks if the game is won.
     *
     * This function determines whether the game is won by checking if all non-mine squares have been revealed.
     * It iterates through each cell on the game board and checks if it is a non-mine square that has not been revealed.
     * If any such square is found, it indicates that the game is not yet won.
     *
     * @return true if all non-mine squares have been revealed and the game is won.
     * @return false if there is at least one non-mine square that has not been revealed.
     */

    bool is_game_won() const
    {
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (!minefield[row][col] && !revealed[row][col])
                {
                    return false; // There is a non-mine square that has not been revealed
                }
            }
        }
        return true; // All non-mine squares have been revealed, game is won
    }

    /**
     * @brief Reveals a cell on the game board.
     *
     * This function reveals the specified cell on the game board. If the cell contains a mine, it is revealed, indicating
     * that the game is lost. If the cell does not contain a mine, it is revealed along with its neighboring cells that
     * also do not contain mines. The function uses a depth-first search algorithm to reveal connected cells with zero mine
     * counts.
     *
     * @param row The row index of the cell to be revealed.
     * @param col The column index of the cell to be revealed.
     */

    void reveal(int row, int col)
    {
        if (minefield[row][col])
        {
            revealed[row][col] = true;
        }
        else
        {
            std::vector<std::pair<int, int>> stack = {{row, col}};
            while (!stack.empty())
            {
                int r = stack.back().first;
                int c = stack.back().second;
                stack.pop_back();
                if (revealed[r][c])
                {
                    continue;
                }
                revealed[r][c] = true;
                if (counts[r][c] == 0)
                {
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (r + i >= 0 && r + i < rows &&
                                    c + j >= 0 && c + j < cols)
                            {
                                stack.push_back({r + i, c + j});
                            }
                        }
                    }
                }
            }
        }
    }
};

/**
 * @brief The entry point of the Minesweeper game.
 *
 * @return int The exit status of the program.
 */

int main()
{
    std::cout << "Hello! Welcome to Minesweeper!" << std::endl << std::endl;
    int rows, cols, num_mines;
    std::cout << "Suggested levels of difficulty:" << std::endl << std::endl;
    std::cout << "Easy (9x9 grid, 10 mines)" << std::endl;
    std::cout << "Medium (16x16 grid, 40 mines)" << std::endl;
    std::cout << "Hard (16x30 grid, 99 mines)" << std::endl << std::endl;
    std::cout << "Insert your preferences below:" << std::endl << std::endl;
    std::cout << "Enter number of rows: ";
    std::cin >> rows;
    std::cout << "Enter number of columns: ";
    std::cin >> cols;
    std::cout << "Enter number of mines: ";
    std::cin >> num_mines;

    Minesweeper game(rows, cols, num_mines);
    game.print();

    while (!game.is_game_over())
    {
        int row, col;
        std::cout << "Enter row and column to reveal: ";
        std::cin >> row >> col;
        game.reveal(row, col);
        game.print();

        if (game.is_game_won())
        {
            std::cout << "Congratulations! You won the game!" << std::endl;
            break;
        }

        if (game.is_game_over())
        {
            std::cout << "You Lost! Game over." << std::endl;
            break;
        }
    }

    return 0;
}
