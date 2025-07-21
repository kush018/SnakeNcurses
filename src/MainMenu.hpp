#pragma once

#include <ncurses.h>
#include <list>
#include <utility>
#include <string>

#include "FrameBare.hpp"

#define SNAKE_SIZE 10 

#define TITLE_ROW 4
#define PLAY_ROW 7
#define HELP_ROW 9
#define QUIT_ROW 11
#define SCORE_ROW 13

class MainMenu: public FrameBare {
public:
    WINDOW* win;
    int yMax, xMax;

    int nRows, nCols;

    bool isDirty;

    bool terminated;

    chtype snakeHeadChar, snakeBodyChar;

    MainMenu();

    ~MainMenu();

    void refreshLoop();
    void eventLoop(int event);

    void initializeSnake(int snakeSize);
    void displayMenu();

    std::list<std::pair<int, int>> snake;   
    void displayCell(int row, int col, chtype ch);
    bool pushHead(); 
    void pullTail(); 
    void step(); 

    bool on = true;

    enum highlighted {PLAY=0, HELP=1, QUIT=2};
    int currentlyHighlighted;

    enum alignment {LEFT, RIGHT, CENTER};
    void print(std::string text, int y, alignment align);
    void clearLine(int y);


    int64_t blinkToggleTimeMicro = 0;
};