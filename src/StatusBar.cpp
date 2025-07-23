#include "StatusBar.hpp"

StatusBar::StatusBar(int startY, int startX, int size) {
    win = newwin(1, size, startY, startX);
    this->size = size;

    wattron(win, A_REVERSE);

    clear();

    isDirty = true;
    refreshLoop();
}

void StatusBar::clear() {
    for (int i = 0; i < size; i++) {
        mvwaddch(win, 0, i, ' ');
    }
    isDirty = true;
}

void StatusBar::printLine(std::string text, alignment align) {
    text = text.substr(0, size);
    // now text is truncated if the size exceeds the size
    int textSize = text.size();
    switch (align) {
        case LEFT:
            // easiest case
            mvwprintw(win, 0, 0, "%s", text.c_str());
            break;
        case CENTER:
            mvwprintw(win, 0, (size-textSize) / 2, "%s", text.c_str());
            break;
        case RIGHT:
            mvwprintw(win, 0, size-textSize, "%s", text.c_str());
    }
    isDirty = true;
}

void StatusBar::refreshLoop() {
    if (isDirty == false) return; // no need to do anything
    wrefresh(win);
    isDirty = false;
}

void StatusBar::eventLoop(int event) {
    
}

StatusBar::~StatusBar() {
    delwin(win);
}
