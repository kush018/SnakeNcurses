#pragma once

#include <ncurses.h>
#include <list>
#include <utility>

#define CELL_HEIGHT 1
#define CELL_WIDTH 2

#define INITIAL_SNAKE_SIZE 4

#define SLEEP_TIME_MS 150

/**
 * (y, x) represents the actual coordinates in the window.
 * (row, col) represents the coordinates in terms of the cell.
 * A cell is essentially a block of terminal characters.
 * All the functions are non-blocking
 */
class SnakeGame {
public:
    /// @brief The window where the game is displayed
    WINDOW* win;

    int score;

    int yMax, xMax; 
    int nRows, nCols;

    /// @brief Snake is a list of (row, col) pairs. The first element is the tail and the last element is the head.
    /// This list always reflects what is actually displayed in the WINDOW* win.
    std::list<std::pair<int, int>> snake;
    /// @brief The coordinates of the apple in terms of (row, col)
    /// This always reflects what is actually displayed in the WINDOW* win.
    std::pair<int, int> apple;

    /// @brief The direction of movement of the snake head
    int rowMovement, colMovement;

    /// @brief Game over, if the snake collides with itself or goes out of bounds
    bool alive;

    /// @brief Characters used to display the snake and apple
    chtype snakeBodyChar, snakeHeadChar, appleChar, deadSnakeChar;

    /// @brief The time when this widget should wake up
    int64_t wakeUpTimeMicro;

    /// @brief Records events even while this widget is sleeping
    std::list<int> eventQueue;

    /// @brief Creates the window of the snakegame by itself
    SnakeGame(int startY, int startX, int nRows, int nCols); 

    /// @brief Places apple at a random location (not in corners or on the snake)
    /// Uses displayCell calls. Does not refresh the window.
    void placeApple(); 

    /// @brief Displays ch into the cell (row, col) by repeating ch for every character in the cell
    /// Does not refresh the window.
    void displayCell(int row, int col, chtype ch); 

    /// @brief Pushes the head of the snake to the next cell in the direction of movement
    /// Does not refresh the window.
    /// If the pushed head will collide, nothing happens other than the old head being drawn with snakeBodyChar instead of snakeHeadChar.
    void pushHead(); 

    /// @brief Removes the tail of the snake. Does not refresh the window.
    void pullTail(); 

    /// @brief Moves the snake one step and refreshes the windows.
    /// Uses pushHead and pullTail calls.
    void step(); 

    /// @brief Non blocking sleep for the given time in milliseconds.
    void sleep_ms(int time); 

    /// @brief To be used with sleep_ms. True, if the widget should wake up.
    bool isAwake();

    /// @brief Colors the snake with deadSnakeChar (as an animation)
    void deathAnimationStep();

    /// @brief Main event loop of this widget. Refreshes on every call.
    /// Only point of contact with the main function
    void eventLoop(int event); 

    void eventHandler();
};
