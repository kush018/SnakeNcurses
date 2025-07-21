#pragma once

#include <ncurses.h>
#include <string>

#include "StatusBar.hpp"
#include "SnakeGame.hpp"
#include "FrameBare.hpp"

class SnakeApp: public FrameBare {
public:

    int xMax, yMax;

    SnakeGame* snakeGame;
    StatusBar* statusBarTop;
    StatusBar* statusBarBottom;

    int score = 0;
    bool isAlive = true;

    SnakeApp(int startY, int startX, int nRows, int nCols);

    void refreshLoop();
    void eventLoop(int event);

    ~SnakeApp();
};