#pragma once

#include <ncurses.h>
#include <cassert>
#include <string>

#include "FrameBare.hpp"
#include "Configuration.hpp"
#include "ColorPairs.hpp"

class GameOverPopup: public FrameBare {
public:
    bool terminated = false;
    bool isDirty = true;
    int score;

    WINDOW* win;

    chtype titleChar;
    int titleBarSize;
    int sizeY, sizeX;
    int titleY, gameOverY, yourScoreY, okY;
    int animationState = 0;
    int animationDirection = 1;

    int secondsRemaining;
    bool buttonHighlighted;

    int64_t countdownTimeMicro = 0;
    int64_t animationTimeMicro = 0;
    int64_t blinkTimeMicro = 0;

    GameOverPopup(int startY, int startX, int sizeY, int sizeX, int score);

    enum alignment {LEFT, CENTER, RIGHT};
    void printLine(std::string text, int y, int align);
    void clearLine(int y);

    void updateDisplay();

    void eventLoop(int event) override;
    void refreshLoop() override;
};