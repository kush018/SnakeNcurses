#include <ncurses.h>
#include <chrono>

#include "FrameBare.hpp"

int64_t FrameBare::timeNowMicro() {

    int64_t timeNow = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now()
        .time_since_epoch()
    ).count();

    return timeNow;
}

void FrameBare::sleep_ms(int time) {
    wakeUpTimeMicro = timeNowMicro() + time * 1000;

}

bool FrameBare::isAwake() {
    return timeNowMicro() >= wakeUpTimeMicro;
}

int FrameBare::popEvent() {
    int event;
    if (!eventQueue.empty()) {
        event = eventQueue.front();
        eventQueue.pop_front();
    } else {
        event = ERR;
    }
    return event;
}

void FrameBare::eventLoop(int event) {
    if (event != ERR) {
        eventQueue.push_back(event);
    }

    if (eventQueue.size() > eventBufferSize) {
        eventQueue.pop_front();
    }
}

void FrameBare::mainLoop(int event) {

    eventLoop(event);
    refreshLoop();
}

void FrameBare::refreshLoop() {

}