#include "SnakeApp.hpp"

SnakeApp::SnakeApp() {

    int nRows = N_ROWS_GAME_GRID;
    int nCols = N_COLS_GAME_GRID;

    int yMax = (2 + nRows * CELL_HEIGHT) + 2;
    int xMax = 2 + nCols * CELL_WIDTH;

    int screenY, screenX; // size of terminal screen
    getmaxyx(stdscr, screenY, screenX);

    int startY = (screenY-yMax)/2;
    int startX = (screenX-xMax)/2;

    snakeGame = new SnakeGame(startY + 1, startX);
    statusBarTop = new StatusBar(startY, startX, snakeGame->xMax);
    statusBarBottom = new StatusBar(startY + snakeGame->yMax, startX, snakeGame->xMax);

    statusBarTop->print("SNAKE GAME", StatusBar::CENTER);
    statusBarBottom->print("SCORE: " + std::to_string(score), StatusBar::CENTER);

    refreshLoop();
};

void SnakeApp::refreshLoop() {
    snakeGame->refreshLoop();
    statusBarTop->refreshLoop();
    statusBarBottom->refreshLoop();
}

void SnakeApp::eventLoop(int event) {
    snakeGame->eventLoop(event);

    terminated = snakeGame->terminated; // terminated if snake game is terminated

    if (score != snakeGame->score) {
        score = snakeGame->score;
        statusBarBottom->clear();
        statusBarBottom->print("SCORE: " + std::to_string(score), StatusBar::CENTER);
    }

    if (isAlive != snakeGame->alive) {
        isAlive = snakeGame->alive;
        statusBarTop->clear();
        if (isAlive) {
            statusBarTop->print("SNAKE GAME", StatusBar::CENTER);
        } else {
            statusBarTop->print("GAME OVER", StatusBar::CENTER);
        }
    }

}

SnakeApp::~SnakeApp() {
    delete snakeGame;
    delete statusBarTop;
    delete statusBarBottom;
}
