#pragma once

#include <string>

#include "FrameBare.hpp"

class StatusBar : public FrameBare {
public:

    WINDOW* win;
    void refreshLoop();

    enum alignment {LEFT, RIGHT, CENTER};

    int size;

    StatusBar(int startY, int startX, int size); 

    void clear(); 

    void print(std::string text, alignment align); 

    ~StatusBar();

};