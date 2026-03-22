#pragma once
#include "input.hh"
#include <pico/types.h>
#include "hw/display.hh"
#include "hw/temp_dht.hh"

enum CurrentMode{
    USUAL,
    SET_ALARM,
    SYNC_TIME
};

struct App{
    DisplayManager& displayManager;
    InputManager& inputManager;
    datetime_t alarm_timer = {};
    char temperature[10] = {0};
    CurrentMode currentMode = SYNC_TIME;

    App(DisplayManager& displayManager,InputManager& inputManager):displayManager(displayManager),inputManager(inputManager){
    }

    void dispatch(std::optional<TempHumidityMeasurement>&);

    private:
    void dispatch_usual(Input input, std::optional<TempHumidityMeasurement>&);
    void dispatch_sync(Input input);
    void transition(CurrentMode newState){
        this->currentMode = newState;
    }
};
extern App app_context_instance;
