#pragma once

#include <ncurses.h>
#include <list>
#include <utility>
#include <string>
#include <cassert>

#include "FrameBare.hpp"
#include "Configuration.hpp"
#include "ColorPairs.hpp"

/**
 * y and x is always with respect to the complete imaginary window that 
 * MainMenu creates (the full one including borders and everything)
 */
class MainMenu: public FrameBare {
public:
    bool terminated; // hand control to the parent

    WINDOW* win;
    int yMax, xMax; // size occupied by the MainMenu window


    bool isDirty;
    void refreshLoop() override;

    void eventLoop(int event) override;

    MainMenu();

    void displayMenu();

    bool on = true; // is currentlyHighlighed actually highlighed (for blinking purposes)
    enum highlighted {PLAY=0, HELP=1, QUIT=2};
    int currentlyHighlighted;

    enum alignment {LEFT, RIGHT, CENTER};
    void printLine(std::string text, int y, int align);
    void clearLine(int y);
    int64_t blinkToggleTimeMicro = 0;

    int nRows, nCols;
    chtype snakeHeadChar, snakeBodyChar;
    std::list<std::pair<int, int>> snake;   

    int64_t snakeAnimationTimeMicro = 0;
    void initializeSnake(int snakeSize);
    void displayCell(int row, int col, chtype ch);
    void pushHead(); 
    void pullTail(); 

    ~MainMenu();
};