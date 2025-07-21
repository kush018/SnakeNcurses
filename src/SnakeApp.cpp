#include <ncurses.h>

#include "SnakeApp.hpp"
#include "FrameBare.hpp"

#include "Configuration.hpp"

SnakeApp::SnakeApp() {

    int nRows = 15;
    int nCols = 15;

    int yMax = (2 + nRows * CELL_HEIGHT) + 2;
    int xMax = 2 + nCols * CELL_WIDTH;

    int screenY, screenX;
    getmaxyx(stdscr, screenY, screenX);

    int startY = (screenY-yMax)/2;
    int startX = (screenX-xMax)/2;

    snakeGame = new SnakeGame(startY + 1, startX, nRows, nCols);
    statusBarTop = new StatusBar(startY, startX, snakeGame->xMax);
    statusBarBottom = new StatusBar(startY + snakeGame->yMax, startX, snakeGame->xMax);

    statusBarTop->print("SNAKE GAME", StatusBar::CENTER);
    statusBarBottom->print("SCORE: " + std::to_string(score), StatusBar::CENTER);

    sleepIntervalMs = 0;

    eventBufferSize = 0;

    refreshLoop();
};

void SnakeApp::refreshLoop() {
    snakeGame->refreshLoop();
    statusBarTop->refreshLoop();
    statusBarBottom->refreshLoop();
}

void SnakeApp::eventLoop(int event) {
    snakeGame->eventLoop(event);

    terminated = snakeGame->terminated;

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
