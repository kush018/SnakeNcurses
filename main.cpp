#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "src/SnakeGame.hpp"

void cleanup(int sig) {
    endwin();
    printf("Program crash\n");
    exit(1);
}

int main() {

    signal(SIGINT, cleanup); // ctrl+c
    signal(SIGTERM, cleanup); // process killed
    signal(SIGSEGV, cleanup); // seg fault

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    if (has_colors()) start_color();

    keypad(stdscr, true);
    nodelay(stdscr, true);

    refresh();
    SnakeGame game(0, 0, 15, 15);

    while (game.alive) {
        int event = getch();
        game.eventLoop(event);
    }

    getch();
    endwin();
}