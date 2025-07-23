#include "SnakeGame.hpp"

SnakeGame::SnakeGame(int startY, int startX) {
    this->nRows = N_ROWS_GAME_GRID;
    this->nCols = N_COLS_GAME_GRID;        

    this->yMax = 2 + nRows * CELL_HEIGHT;
    this->xMax = 2 + nCols * CELL_WIDTH;

    this->win = newwin(yMax, xMax, startY, startX);

    box(win, 0, 0);

    snakeBodyChar = '#' | COLOR_PAIR(BLACK_ON_GREEN);
    snakeHeadChar = '\'' | COLOR_PAIR(RED_ON_YELLOW);
    appleChar = '@' | COLOR_PAIR(CYAN_ON_MAGENTA);
    deadSnakeChar = 'X' | COLOR_PAIR(WHITE_ON_RED);

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

    srand(timeNowMicro());
    placeApple();

    isDirty = true;
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
    isDirty = true;
}

void SnakeGame::pushHead() {
    // convert the head to the body char
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
    if (snake.empty()) {
        terminated = true; // nothing left to do anymore
        return;
    }
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
        case KEY_UP: case 'w': case 'W':
            newRowMovement = -1; newColMovement = 0; break;
        case KEY_DOWN: case 's': case 'S':
            newRowMovement = 1; newColMovement = 0; break;
        case KEY_RIGHT: case 'd': case 'D':
            newRowMovement = 0; newColMovement = 1; break;
        case KEY_LEFT: case 'a': case 'A':
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

    wakeUpTimeMicro = timeNowMicro() + SNAKE_GAME_MOVEMENT_DELAY_MS * 1000;
}

void SnakeGame::eventHandlerDead() {
    deathAnimationStep();

    wakeUpTimeMicro = timeNowMicro() + SNAKE_DEATH_ANIMATION_DELAY_MS * 1000;
}

void SnakeGame::eventLoop(int event) {
    if (terminated) return; // do nothing

    queueEvent(event);

    bool isAwake = timeNowMicro() >= wakeUpTimeMicro;
    if (!isAwake) {
        // if the widget is asleep, it should not process any events
        return;
    }

    if (alive) {
        eventHandlerAlive();
    } else {
        eventHandlerDead();
    }

}

void SnakeGame::refreshLoop() {
    if (isDirty == false) return; // no need to do anything
    wrefresh(win);
    isDirty = false;
}

SnakeGame::~SnakeGame() {
    delwin(win);
}
