#include "GameOverPopup.hpp"

GameOverPopup::GameOverPopup(int startY, int startX, int sizeY, int sizeX, int score) {
    assert(sizeY >= 6);

    this->sizeY = sizeY;
    this->sizeX = sizeX;

    this->score = score;

    titleY = 1;
    gameOverY = 2;
    yourScoreY = sizeY-3;
    okY = sizeY-2;

    win = newwin(sizeY, sizeX, startY, startX);

    box(win, 0, 0);

    countdownTimeMicro = timeNowMicro();
    animationTimeMicro = timeNowMicro();
    blinkTimeMicro = timeNowMicro();

    animationState = 1;
    animationDirection = 1;
    titleChar = ' ' | COLOR_PAIR(BLACK_ON_WHITE);
    buttonHighlighted = true;
    secondsRemaining = POPUP_SECONDS_REMAINING;
    titleBarSize = POPUP_TITLE_BAR_SIZE;
    updateDisplay();

    isDirty = true;
    
}

void GameOverPopup::updateDisplay() {
    clearLine(titleY);
    clearLine(gameOverY);
    clearLine(yourScoreY);
    clearLine(okY);

    for (int x = animationState; x < animationState + titleBarSize; x++) {
        mvwaddch(win, titleY, x, titleChar);
    }

    printLine("GAME OVER!", gameOverY, CENTER);
    printLine("YOUR SCORE: " + std::to_string(score), yourScoreY, CENTER);

    if (buttonHighlighted) wattron(win, A_REVERSE);
    printLine("OK (" + std::to_string(secondsRemaining) + "s)", okY, CENTER);
    if (buttonHighlighted) wattroff(win, A_REVERSE);

    isDirty = true;
}

void GameOverPopup::clearLine(int y) {
    int startX = 1;
    int maxLen = sizeX - 2;
    assert(1 <= y && y < sizeY - 1);

    for (int x = startX; x < startX + maxLen; x++) {
        mvwaddch(win, y, x, ' ');
    }
    isDirty = true;
}

void GameOverPopup::printLine(std::string text, int y, int align) {
    int startX = 1; // x coordinate where the border just ends
    int maxLen = sizeX - 2; // maximum writing length
    assert(1 <= y && y < sizeY - 1); // ensure that y is not on the border
    text = text.substr(0, maxLen); // now text is truncated if the size exceeds the size
    int textSize = text.size();
    switch (align) {
        case LEFT:
            // easiest case
            mvwprintw(win, y, startX, "%s", text.c_str());
            break;
        case CENTER:
            mvwprintw(win, y, startX + (maxLen-textSize)/2, "%s", text.c_str());
            break;
        case RIGHT:
            mvwprintw(win, y, startX + (maxLen-textSize), "%s", text.c_str());
    }
    isDirty = true;
}

void GameOverPopup::eventLoop(int event) {
    if (terminated) return; 

    if (event != ERR || secondsRemaining <= 0) {
        terminated = true;
        return;
    }

    if (timeNowMicro() >= countdownTimeMicro) {
        secondsRemaining--;
        countdownTimeMicro += 1000 * 1000;
        updateDisplay();
    }
    if (timeNowMicro() >= animationTimeMicro) {
        assert(abs(animationDirection) == 1);
        if (animationDirection == 1) {
            if (animationState < sizeX-1-titleBarSize) {
                animationState++;
            } else {
                animationDirection = -1;
                animationState--;
            }
        } else { // if (animationDirection == -1)
            if (animationState > 1) {
                animationState--;
            } else {
                animationDirection = 1;
                animationState++;
            }
        }
        animationTimeMicro += POPUP_ANIMATION_DELAY_MS * 1000;
        updateDisplay();
    }
    if (timeNowMicro() >= blinkTimeMicro) {
        buttonHighlighted = !buttonHighlighted;
        blinkTimeMicro += POPUP_BLINK_DELAY_MS * 1000;
        updateDisplay();
    }
}

void GameOverPopup::refreshLoop() {
    if (isDirty) {
        wrefresh(win);
        isDirty = false;
    }
}

GameOverPopup::~GameOverPopup() {
    delwin(win);
}


