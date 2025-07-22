#pragma once

#include <string>
#include <ncurses.h>

#include "FrameBare.hpp"

class StatusBar : public FrameBare {
public:

    WINDOW* win;
    int size;

    bool isDirty = true;
    void refreshLoop() override;

    void eventLoop(int event) override;

    StatusBar(int startY, int startX, int size); 

    void clear(); 

    enum alignment {LEFT, RIGHT, CENTER};
    void print(std::string text, alignment align); 

    ~StatusBar();

};