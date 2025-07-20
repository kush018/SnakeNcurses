#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

#include "src/SnakeGame.hpp"

int main() {

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    if (has_colors()) start_color();

    keypad(stdscr, true);
    nodelay(stdscr, true);

    refresh();
    SnakeGame game(0, 0, 15, 15);

    while (true) {
        int event = getch();
        game.mainLoop(event);
    }

    nodelay(stdscr, false);
    getch();
    endwin();
}