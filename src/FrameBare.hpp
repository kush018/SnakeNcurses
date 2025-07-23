#pragma once

#include <ncurses.h>
#include <list>
#include <chrono>

class FrameBare {
public:

    /// @brief Current time in microseconds
    int64_t timeNowMicro();

    /// @brief Records events even while this widget is sleeping
    std::list<int> eventQueue;
    /// @brief Delete and return the oldest elment in eventQueue (FIFO)
    int popEvent();
    /// @brief Adds event to eventQueue. If eventQueue exceeds size eventBufferSize, it deletes oldest event
    void queueEvent(int event);
    /// @brief Main event loop of this widget. Point of contact with the main function
    virtual void eventLoop(int event); 
    /// @brief No other function calls refresh or wrefresh except this one
    virtual void refreshLoop();

    int eventBufferSize = 10;
};
