#include "SnakeApp.hpp"

SnakeApp::SnakeApp() {

    int nRows = N_ROWS_GAME_GRID;
    int nCols = N_COLS_GAME_GRID;

    yMax = (2 + nRows * CELL_HEIGHT) + 2;
    xMax = 2 + nCols * CELL_WIDTH;

    int screenY, screenX; // size of terminal screen
    getmaxyx(stdscr, screenY, screenX);

    startY = (screenY-yMax)/2;
    startX = (screenX-xMax)/2;

    snakeGame = new SnakeGame(startY + 1, startX);
    statusBarTop = new StatusBar(startY, startX, snakeGame->xMax);
    statusBarBottom = new StatusBar(startY + snakeGame->yMax, startX, snakeGame->xMax);

    statusBarTop->printLine("SNAKE GAME", StatusBar::CENTER);
    statusBarBottom->printLine("SCORE: " + std::to_string(score), StatusBar::CENTER);

    refreshLoop();
};

void SnakeApp::refreshLoop() {
    if (snakeGame->terminated == false) {
        snakeGame->refreshLoop();
        statusBarTop->refreshLoop();
        statusBarBottom->refreshLoop();
    } else {
        if (gameOverPopup != nullptr) gameOverPopup->refreshLoop();
    }

}

void SnakeApp::eventLoop(int event) {
    if (terminated) return;

    if (snakeGame->terminated == false) {
        snakeGame->eventLoop(event);

        if (score != snakeGame->score) {
            score = snakeGame->score;
            statusBarBottom->clear();
            statusBarBottom->printLine("SCORE: " + std::to_string(score), StatusBar::CENTER);
        }

        if (isAlive != snakeGame->alive) {
            isAlive = snakeGame->alive;
            statusBarTop->clear();
            if (isAlive) {
                statusBarTop->printLine("SNAKE GAME", StatusBar::CENTER);
            } else {
                statusBarTop->printLine("GAME OVER", StatusBar::CENTER);
            }
        }
    } else {
        if (gameOverPopup == nullptr) {
            int popupSizeY = 6;
            int popupSizeX = xMax - 4;
            
            gameOverPopup = new GameOverPopup(startY + (yMax - popupSizeY)/2, startX + (xMax - popupSizeX)/2, popupSizeY, popupSizeX, snakeGame->score);
        }
        gameOverPopup->eventLoop(event);
        if (gameOverPopup->terminated) terminated = true;
    }

}

SnakeApp::~SnakeApp() {
    if (snakeGame != nullptr) delete snakeGame;
    if (statusBarTop != nullptr) delete statusBarTop;
    if (statusBarBottom != nullptr) delete statusBarBottom;
    if (gameOverPopup != nullptr) gameOverPopup;
}
