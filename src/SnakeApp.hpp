#pragma once

#include <ncurses.h>
#include <string>

#include "StatusBar.hpp"
#include "SnakeGame.hpp"
#include "GameOverPopup.hpp"
#include "FrameBare.hpp"

#include "Configuration.hpp"

/**
 * y and x is always with respect to the complete imaginary window that 
 * SnakeApp creates (the full one including statusbars and everything)
 */
class SnakeApp: public FrameBare {
public:
    bool terminated = false;

    int64_t wakeUpTimeMicro = 0;

    SnakeGame* snakeGame = nullptr;
    StatusBar* statusBarTop = nullptr;
    StatusBar* statusBarBottom = nullptr;
    GameOverPopup* gameOverPopup = nullptr;

    int xMax, yMax; // Size occupied by whole SnakeApp window
    int startY, startX;

    int score = 0;
    bool isAlive = true;

    SnakeApp();

    void refreshLoop() override;
    void eventLoop(int event) override;

    ~SnakeApp();
};