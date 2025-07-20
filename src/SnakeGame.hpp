#pragma once

#include <ncurses.h>
#include <list>
#include <utility>

#include "Frame.hpp"

#define CELL_HEIGHT 1
#define CELL_WIDTH 2

#define INITIAL_SNAKE_SIZE 4

#define SNAKE_SPEED 150

/**
 * (y, x) represents the actual coordinates in the window.
 * (row, col) represents the coordinates in terms of the cell.
 * A cell is essentially a block of terminal characters.
 * All the functions are non-blocking
 * All functions do NOT refresh the window except eventLoop and the constructor
 */
class SnakeGame: public Frame {
public:

    /// @brief Main event loop of this widget. Refreshes at the end of the function
    /// Only point of contact with the main function
    void eventLoop(int event); 

    void eventHandlerAlive();
    void eventHandlerDead();

    int yMax, xMax; 
    int nRows, nCols;
    int score;
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

    /// @brief Creates the window of the snakegame by itself
    SnakeGame(int startY, int startX, int nRows, int nCols); 
    /// @brief Places apple at a random location (not in corners or on the snake)
    /// Uses displayCell calls.
    void placeApple(); 
    /// @brief Displays ch into the cell (row, col) by repeating ch for every character in the cell
    void displayCell(int row, int col, chtype ch); 
    /// @brief Pushes the head of the snake to the next cell in the direction of movement
    /// If the pushed head will collide, nothing happens other than the old head being drawn with snakeBodyChar instead of snakeHeadChar.
    void pushHead(); 
    /// @brief Removes the tail of the snake.
    void pullTail(); 
    /// @brief Moves the snake one step. Uses pushHead and pullTail calls.
    void step(); 
    /// @brief Colors the snake with deadSnakeChar (as an animation)
    void deathAnimationStep();

};
