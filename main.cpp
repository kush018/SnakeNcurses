#include <ncurses.h>
#include <stdio.h>

#include "src/SnakeApp.hpp"

int main() {

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    if (has_colors()) start_color();

    keypad(stdscr, true);
    nodelay(stdscr, true);

    refresh();
    SnakeApp snakeApp(0, 0, 15, 15);

    while (true) {
        int event = getch();
        snakeApp.mainLoop(event);
    }

    nodelay(stdscr, false);
    getch();
    endwin();
}