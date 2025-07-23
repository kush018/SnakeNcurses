#include <ncurses.h>
#include <stdio.h>

#include "src/SnakeApp.hpp"
#include "src/MainMenu.hpp"

#include "src/ColorPairs.hpp"

int main() {

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    if (has_colors()) start_color();

    init_pair(BLACK_ON_GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(RED_ON_YELLOW, COLOR_RED, COLOR_YELLOW);
    init_pair(CYAN_ON_MAGENTA, COLOR_CYAN, COLOR_MAGENTA);
    init_pair(WHITE_ON_RED, COLOR_WHITE, COLOR_RED);
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);

    keypad(stdscr, true);
    nodelay(stdscr, true);

    enum {MAIN_MENU, SNAKE_APP};


    refresh();
    MainMenu* mainMenu = nullptr;
    SnakeApp* snakeApp = nullptr;

    int context = MAIN_MENU;

    while (true) {
        int event = getch();
        switch (context) {
            case MAIN_MENU:
                if (mainMenu == nullptr) mainMenu = new MainMenu();
                mainMenu->eventLoop(event);
                mainMenu->refreshLoop();
                if (mainMenu->terminated) {
                    clear();
                    switch (mainMenu->currentlyHighlighted) {
                        case mainMenu->PLAY:
                            context = SNAKE_APP;
                            break;
                        case mainMenu->HELP:
                            context = MAIN_MENU;
                            break;
                        case mainMenu->QUIT:
                            if (mainMenu != nullptr) delete mainMenu;
                            if (snakeApp != nullptr) delete snakeApp;
                            endwin();
                            return 0;
                    }
                    delete mainMenu;
                    mainMenu = nullptr;
                }
                break;
            case SNAKE_APP:
                if (snakeApp == nullptr) snakeApp = new SnakeApp();
                snakeApp->eventLoop(event);
                snakeApp->refreshLoop();
                if (snakeApp->terminated) {
                    clear();
                    context = MAIN_MENU;
                    delete snakeApp;
                    snakeApp = nullptr;
                }
                break;
        }
    }

    if (mainMenu != nullptr) delete mainMenu;
    if (snakeApp != nullptr) delete snakeApp;
    endwin();

}

