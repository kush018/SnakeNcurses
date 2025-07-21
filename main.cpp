#include <ncurses.h>
#include <stdio.h>

#include "src/SnakeApp.hpp"
#include "src/MainMenu.hpp"

int main() {

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    if (has_colors()) start_color();

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
                mainMenu->mainLoop(event);
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
                            endwin();
                            return 0;
                    }
                    delete mainMenu;
                    mainMenu = nullptr;
                }
                break;
            case SNAKE_APP:
                if (snakeApp == nullptr) snakeApp = new SnakeApp();
                snakeApp->mainLoop(event);
                if (snakeApp->terminated) {
                    clear();
                    context = MAIN_MENU;
                    delete snakeApp;
                    snakeApp = nullptr;
                }
                break;
        }
    }


    endwin();

}

