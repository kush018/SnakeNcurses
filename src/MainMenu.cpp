#include <ncurses.h>
#include <cassert>
#include <string>

#include "MainMenu.hpp"

#include "Configuration.hpp"

MainMenu::MainMenu() {
    nRows = 15;
    nCols = 15;

    terminated = false;

    currentlyHighlighted = PLAY;

    yMax = 2 + nRows * CELL_HEIGHT;
    xMax = 2 + nCols * CELL_WIDTH;

    isDirty = true;

    int yScreen, xScreen;
    getmaxyx(stdscr, yScreen, xScreen);

    win = newwin(yMax, xMax, (yScreen-yMax)/2, (xScreen-xMax)/2);
    box(win, 0, 0);

    init_pair(1, COLOR_BLACK, COLOR_GREEN); // body of the snake is this colorpair
    init_pair(2, COLOR_RED, COLOR_YELLOW); // head of the snake
    snakeBodyChar = '#' | COLOR_PAIR(1);
    snakeHeadChar = '\'' | COLOR_PAIR(2);

    initializeSnake(SNAKE_SIZE);

    displayMenu();

    refreshLoop();
}

void MainMenu::displayMenu() {
    /*
    SNAKE GAME

    PLAY
    HELP
    QUIT
    
    HIGH SCORE
    */

    clearLine(TITLE_ROW);
    clearLine(PLAY_ROW);
    clearLine(HELP_ROW);
    clearLine(QUIT_ROW);

    wattron(win, A_BOLD | A_UNDERLINE);
    print("SNAKE GAME", TITLE_ROW, CENTER); // title
    wattroff(win, A_BOLD | A_UNDERLINE);

    if (currentlyHighlighted == PLAY && on) wattron(win, A_REVERSE);
    print("(P)LAY", PLAY_ROW, CENTER);
    if (currentlyHighlighted == PLAY && on) wattroff(win, A_REVERSE);
    
    
    if (currentlyHighlighted == HELP && on) wattron(win, A_REVERSE);
    print("(H)ELP", HELP_ROW, CENTER);
    if (currentlyHighlighted == HELP && on) wattroff(win, A_REVERSE);

    if (currentlyHighlighted == QUIT && on) wattron(win, A_REVERSE);
    print("(Q)UIT", QUIT_ROW, CENTER);
    if (currentlyHighlighted == QUIT && on) wattroff(win, A_REVERSE);
}

void MainMenu::initializeSnake(int snakeSize) {
    displayCell(nRows-1, nCols-1, snakeHeadChar);
    snake.push_back({nRows-1, nCols-1});
    for (int i = 0; i < snakeSize-1; i++) {
        pushHead();
    }
}

void MainMenu::displayCell(int row, int col, chtype ch) {
    for (int y = 1 + row * CELL_HEIGHT; y < 1 + row * CELL_HEIGHT + CELL_HEIGHT; y++) {
        for (int x = 1 + col * CELL_WIDTH; x < 1 + col * CELL_WIDTH + CELL_WIDTH; x++) {
            mvwaddch(win, y, x, ch);
        }
    }
    isDirty = true;
}

// note that this is for anticlockwise movement
bool MainMenu::pushHead() {

    int rowHead = snake.back().first;
    int colHead = snake.back().second;

    int rowMovement, colMovement;
    if (rowHead < nRows-1 && colHead == 0) {
        rowMovement = 1; colMovement = 0;
    } else if (rowHead == nRows-1 && colHead < nCols-1) {
        rowMovement = 0; colMovement = 1;
    } else if (0 < rowHead && colHead == nCols-1) {
        rowMovement = -1; colMovement = 0;
    } else if (rowHead == 0 && 0 < colHead) {
        rowMovement = 0; colMovement = -1;
    }

    // convert the head to the body color
    displayCell(snake.back().first, snake.back().second, snakeBodyChar);

    // create a new head
    // new coordinates
    rowHead = snake.back().first + rowMovement;
    colHead = snake.back().second + colMovement;

    bool inBounds = 0 <= rowHead && rowHead < nRows && 0 <= colHead && colHead < nCols;
    bool touchingSelf = false;
    for (auto snakePart : snake) {
        if (snakePart.first == rowHead && snakePart.second == colHead) {
            touchingSelf = true;
            break;
        }
    }
    bool alive = inBounds && (!touchingSelf);

    if (alive) {
        // actually create a new head
        snake.push_back({rowHead, colHead});
        displayCell(rowHead, colHead, snakeHeadChar);
    } 

    return alive;
}

void MainMenu::pullTail() {
    // get rid of the tail
    displayCell(snake.front().first, snake.front().second, ' ');
    snake.pop_front();
}

void MainMenu::eventLoop(int event) {
    if (isAwake()) {
        // for snake animation

        pushHead();
        pullTail();

        sleep_ms(100);
    }

    bool touched = false;
    if (timeNowMicro() >= blinkToggleTimeMicro) {
        on = on ? false : true;
        blinkToggleTimeMicro = timeNowMicro() + 1000 * 300;
        touched = true;
    }

    switch (event) {
        case KEY_UP:
            if (currentlyHighlighted > 0) currentlyHighlighted--;
            touched = true;
            break;
        case KEY_DOWN:
            if (currentlyHighlighted < 2) currentlyHighlighted++;
            touched = true;
            break;
        case KEY_ENTER: case '\n': case '\r': case ' ':
            terminated = true;
            break;
        case 'p': case 'P':
            currentlyHighlighted = PLAY;
            terminated = true;
            break;
        case 'h': case 'H':
            currentlyHighlighted = HELP;
            terminated = true;
            break;
        case 'q': case 'Q':
            currentlyHighlighted = QUIT;
            terminated = true;
            break;
    }

    if (touched) displayMenu();
}

void MainMenu::print(std::string text, int y, alignment align) {
    int startX = 1 + CELL_WIDTH;
    int maxLen = xMax - 2 - 2 * CELL_WIDTH;
    assert(1 + CELL_HEIGHT <= y && y < yMax - CELL_HEIGHT);
    text = text.substr(0, maxLen);
    // now text is truncated if the size exceeds the size
    int textSize = text.size();
    switch (align) {
        case LEFT:
            // easiest case
            mvwprintw(win, y, startX, text.c_str());
            break;
        case CENTER:
            mvwprintw(win, y, startX + (maxLen-textSize)/2, text.c_str());
            break;
        case RIGHT:
            mvwprintw(win, y, startX + (maxLen-textSize), text.c_str());
    }
    isDirty = true;
}

void MainMenu::clearLine(int y) {
    int startX = 1 + CELL_WIDTH;
    int maxLen = xMax - 2 - 2 * CELL_WIDTH;
    assert(1 + CELL_HEIGHT <= y && y < yMax - CELL_HEIGHT);

    for (int x = startX; x < startX + maxLen; x++) {
        mvwaddch(win, y, x, ' ');
    }

    isDirty = true;
}

void MainMenu::refreshLoop() {
    if (isDirty == false) return; // no need to do anything
    wrefresh(win);
    isDirty = false;
}

MainMenu::~MainMenu() {
    delwin(win);
}