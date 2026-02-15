#pragma once
#include "input.hh"
#include <pico/types.h>
#include "display.hh"
#include "temp_dht.hh"

enum CurrentMode{
    USUAL,
    SET_ALARM,
    SYNC_TIME
};

struct AppContext{
    DisplayManager& displayManager;
    datetime_t alarm_timer = {};
    char temperature[10] = {0};
    CurrentMode currentMode = SYNC_TIME;

    AppContext(DisplayManager& displayManager):displayManager(displayManager){
    }

    void dispatch(Input input, std::optional<TempHumidityMeasurement>&);

    private:
    void dispatch_usual(Input input, std::optional<TempHumidityMeasurement>&);
    void dispatch_sync(Input input);
    void transition(CurrentMode newState){
        this->currentMode = newState;
    }
};
extern AppContext app_context_instance;
