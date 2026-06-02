#pragma once
#include "input.hh"
#include <pico/types.h>
#include "hw/display.hh"
#include "hw/temp_dht.hh"

enum CurrentMode{
    USUAL,
    DEGRADED_NO_TIME,
    SET_ALARM,
    SYNC_TIME
};

struct App{
    private:
    DisplayManager& displayManager;
    InputManager& inputManager;
    TempSensorInput& tempSensorInput;
    datetime_t alarm_timer = {};
    CurrentMode currentMode = SYNC_TIME;
    uint32_t ticks_in_state = 0;


    void dispatch_usual(Input input, std::optional<TempHumidityMeasurement>&);
    void dispatch_degraded_no_time(Input input, std::optional<TempHumidityMeasurement>&);
    void dispatch_sync(Input input);

    void draw_temp_humidity(std::optional<TempHumidityMeasurement>& env_input);

    inline void transition(CurrentMode newState){
        this->currentMode = newState;
        this->ticks_in_state = 0;
    }

    public:
    App(DisplayManager& displayManager,InputManager& inputManager, TempSensorInput& tempSensorInput):displayManager(displayManager),inputManager(inputManager),tempSensorInput(tempSensorInput){
    }
    void dispatch();
};
