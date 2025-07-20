#include <cassert>
#include <cstdlib>
#include <chrono>
#include <utility>
#include <list>
#include <ncurses.h>

#include "SnakeGame.hpp"
#include "FrameBare.hpp"

SnakeGame::SnakeGame(int startY, int startX, int nRows, int nCols) {
    rowMovement = 0;
    colMovement = 1;

    srand(timeNowMicro());

    sleepIntervalMs = 0;

    score = INITIAL_SNAKE_SIZE;
    alive = true;

    wakeUpTimeMicro = 0; // initially this widget is awake

    init_pair(1, COLOR_BLACK, COLOR_GREEN); // body of the snake is this colorpair
    init_pair(2, COLOR_RED, COLOR_YELLOW); // head of the snake
    init_pair(3, COLOR_CYAN, COLOR_MAGENTA); // apples (yumm!)
    init_pair(4, COLOR_WHITE, COLOR_RED); // dead snake (really sad)
    snakeBodyChar = '#' | COLOR_PAIR(1);
    snakeHeadChar = '\'' | COLOR_PAIR(2);
    appleChar = '@' | COLOR_PAIR(3);
    deadSnakeChar = 'X' | COLOR_PAIR(4);

    assert(nRows >= 2);
    assert(nCols >= INITIAL_SNAKE_SIZE + 1);

    this->nRows = nRows;
    this->nCols = nCols;        

    this->yMax = 2 + nRows * CELL_HEIGHT;
    this->xMax = 2 + nCols * CELL_WIDTH;

    this->win = newwin(yMax, xMax, startY, startX);

    box(win, 0, 0);
    

    for (int i = 0; i < INITIAL_SNAKE_SIZE; i++) {
        snake.push_back({0, i});
        if (i < INITIAL_SNAKE_SIZE - 1) {
            // body of the snake
            displayCell(0, i, snakeBodyChar);
        } else {
            // head of the snake
            displayCell(0, i, snakeHeadChar);
        }
    }

    placeApple();

    refreshLoop();
}

void SnakeGame::placeApple() {
    int rowApple, colApple;
    while (true) {
        rowApple = rand() % nRows;
        colApple = rand() % nCols;
        // the apple should not be present on any part of the snake or at the corners
        bool appleOnSnake = false;
        for (auto snakePart : snake) {
            if (snakePart.first == rowApple && snakePart.second == colApple) {
                appleOnSnake = true;
                break;
            }
        }
        if (appleOnSnake) continue; // retry

        bool isCorner = false;
        for (int row = 0; row < nRows; row += nRows - 1) {
            for (int col = 0; col < nCols; col += nCols - 1) {
                // (row, col) is a corner point
                if (row == rowApple && col == colApple) {
                    isCorner = true;
                    break;
                }
            }
        }

        if (isCorner) continue; // retry

        break; // valid apple location
    }

    apple = {rowApple, colApple};
    displayCell(rowApple, colApple, appleChar);
}



void SnakeGame::displayCell(int row, int col, chtype ch) {
    for (int y = 1 + row * CELL_HEIGHT; y < 1 + row * CELL_HEIGHT + CELL_HEIGHT; y++) {
        for (int x = 1 + col * CELL_WIDTH; x < 1 + col * CELL_WIDTH + CELL_WIDTH; x++) {
            mvwaddch(win, y, x, ch);
        }
    }
}

void SnakeGame::pushHead() {
    // convert the head to the body color
    displayCell(snake.back().first, snake.back().second, snakeBodyChar);

    // create a new head
    // new coordinates
    int rowHead = snake.back().first + rowMovement;
    int colHead = snake.back().second + colMovement;

    bool inBounds = 0 <= rowHead && rowHead < nRows && 0 <= colHead && colHead < nCols;
    bool touchingSelf = false;
    for (auto snakePart : snake) {
        if (snakePart.first == rowHead && snakePart.second == colHead) {
            touchingSelf = true;
            break;
        }
    }
    alive = inBounds && (!touchingSelf);

    if (alive) {
        // actually create a new head
        snake.push_back({rowHead, colHead});
        displayCell(rowHead, colHead, snakeHeadChar);
    } 
}

void SnakeGame::pullTail() {
    // get rid of the tail
    displayCell(snake.front().first, snake.front().second, ' ');
    snake.pop_front();
}

void SnakeGame::step() {

    if (!alive) return;

    // sanity check
    assert(colMovement == 0 || rowMovement == 0);
    assert(abs(colMovement) == 1 || abs(rowMovement) == 1);


    pullTail();

    pushHead();

    if (!alive) {
        return;
    }

    int rowApple = apple.first;
    int colApple = apple.second;
    if (snake.back().first == rowApple && snake.back().second == colApple) {
        // eating an apple 
        // add new head without removing tail
        pushHead();
        if (alive) {
            score++;
            placeApple();
        }
    }


}


void SnakeGame::deathAnimationStep() {
    // go head first
    // head is at the last of snake list
    if (snake.empty()) return;
    auto snakeHead = snake.back();
    int rowHead = snakeHead.first;
    int colHead = snakeHead.second;
    displayCell(rowHead, colHead, deadSnakeChar);
    snake.pop_back();
}

void SnakeGame::eventHandlerAlive() {
    int event = popEvent();

    int newRowMovement, newColMovement;
    newRowMovement = rowMovement;
    newColMovement = colMovement;
    switch (event) {
        case KEY_UP:
            newRowMovement = -1; newColMovement = 0; break;
        case KEY_DOWN:
            newRowMovement = 1; newColMovement = 0; break;
        case KEY_RIGHT:
            newRowMovement = 0; newColMovement = 1; break;
        case KEY_LEFT:
            newRowMovement = 0; newColMovement = -1;
    }

    if (newRowMovement == -rowMovement && newColMovement == -colMovement) {
        // an invalid move
    } else {
        // valid move
        rowMovement = newRowMovement;
        colMovement = newColMovement;
    }

    step();

    sleepIntervalMs = SNAKE_SPEED;
}

void SnakeGame::eventHandlerDead() {
    deathAnimationStep();
    sleepIntervalMs = 100;
}

void SnakeGame::eventLoop(int event) {
    FrameBare::eventLoop(event);

    if (!isAwake()) {
        // if the widget is asleep, it should not process any events
        return;
    }

    if (alive) {
        eventHandlerAlive();
    } else {
        eventHandlerDead();
    }

    sleep_ms(sleepIntervalMs);
}

void SnakeGame::refreshLoop() {
    wrefresh(win);
}

SnakeGame::~SnakeGame() {
    delwin(win);
}
