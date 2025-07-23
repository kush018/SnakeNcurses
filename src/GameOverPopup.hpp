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
    int score;

    WINDOW* win;
    int sizeY, sizeX;

    int titleY, gameOverY, yourScoreY, okY;

    int64_t countdownTimeMicro = 0;
    int64_t animationTimeMicro = 0;
    int64_t blinkTimeMicro = 0;

    chtype titleChar;
    int titleBarSize; // size of the moving white bar thing at the top
    int animationState = 0; // position of the moving bar thing at the top
    int animationDirection = 1; // 1 for moving right, -1 for moving left

    int secondsRemaining; // seconds remaining before it closes automatically

    bool buttonHighlighted; // is the ok button highlighted or not. important for blinking effect

    GameOverPopup(int startY, int startX, int sizeY, int sizeX, int score);

    enum alignment {LEFT, CENTER, RIGHT};
    void printLine(std::string text, int y, int align);
    void clearLine(int y);

    void updateDisplay();

    void eventLoop(int event) override;

    bool isDirty = true;
    void refreshLoop() override;

    ~GameOverPopup();
};