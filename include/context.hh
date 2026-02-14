#pragma once
#include "input.hh"
#include <pico/types.h>

enum CurrentMode{
    USUAL,
    SET_ALARM,
    SYNC_TIME
};

struct AppContext{
    datetime_t alarm_timer = {};
    char temperature[10] = {0};
    CurrentMode currentMode = SYNC_TIME;

    void dispatch(Input input);
    private:
    void dispatch_usual(Input input);
    void dispatch_sync(Input input);
    void transition(CurrentMode newState){
        this->currentMode = newState;
    }
};
extern AppContext app_context_instance;
