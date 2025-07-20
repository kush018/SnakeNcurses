#include "SnakeApp.hpp"
#include "FrameBare.hpp"

SnakeApp::SnakeApp(int startY, int startX, int nRows, int nCols) {
    snakeGame = new SnakeGame(startY + 1, startX, nRows, nCols);
    statusBarTop = new StatusBar(startY, startX, snakeGame->xMax);
    statusBarBottom = new StatusBar(startY + snakeGame->yMax, startX, snakeGame->xMax);

    statusBarTop->print("SNAKE GAME", StatusBar::CENTER);

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

    statusBarBottom->clear();
    statusBarBottom->print("SCORE: " + std::to_string(snakeGame->score), StatusBar::RIGHT);
}

SnakeApp::~SnakeApp() {
    delete snakeGame;
    delete statusBarTop;
    delete statusBarBottom;
}
