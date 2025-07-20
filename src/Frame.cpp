#include <ncurses.h>
#include <list>
#include <chrono>

#include "Frame.hpp"

void Frame::sleep_ms(int time) {
    int64_t timeNowMicro = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now()
        .time_since_epoch()
    ).count();

    wakeUpTimeMicro = timeNowMicro + time * 1000;
}

bool Frame::isAwake() {
    int64_t timeNowMicro = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now()
        .time_since_epoch()
    ).count();
    
    return timeNowMicro >= wakeUpTimeMicro;
}

int Frame::popEvent() {
    int event;
    if (!eventQueue.empty()) {
        event = eventQueue.front();
        eventQueue.pop_front();
    } else {
        event = ERR;
    }
    return event;
}

void Frame::eventLoop(int event) {
    if (event != ERR) {
        eventQueue.push_back(event);
    }
}

void Frame::sleepLoop() {
    if (isAwake()) {
        sleep_ms(sleepIntervalMs);
    }
}

void Frame::refreshLoop() {
    wrefresh(win);
}

void Frame::mainLoop(int event) {
    eventLoop(event);
    sleepLoop();
    refreshLoop();
}