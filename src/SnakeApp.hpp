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

    bool terminated = false;

    int score = 0;
    bool isAlive = true;

    SnakeApp();

    void refreshLoop();
    void eventLoop(int event);

    ~SnakeApp();
};