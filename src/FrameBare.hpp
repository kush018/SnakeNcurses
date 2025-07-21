#pragma once

#include <ncurses.h>
#include <list>

class FrameBare {
public:

    int sleepIntervalMs;
    /// @brief The time when this widget should wake up
    int64_t wakeUpTimeMicro = 0;
    /// @brief Current time in microseconds
    int64_t timeNowMicro();
    /// @brief Non blocking sleep for the given time in milliseconds.
    void sleep_ms(int time); 
    /// @brief To be used with sleep_ms. True, if the widget should wake up.
    bool isAwake();

    /// @brief Records events even while this widget is sleeping
    std::list<int> eventQueue;
    /// @brief Delete and return the oldest elment in eventQueue (FIFO)
    int popEvent();
    /// @brief Main event loop of this widget. Refreshes at the end of the function
    /// Only point of contact with the main function
    virtual void eventLoop(int event); 
    virtual void refreshLoop();
    void mainLoop(int event);

    int eventBufferSize = 10;
};
