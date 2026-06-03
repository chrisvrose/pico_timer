#include "app.hh"
#include "config.hh"
#include "hw/display.hh"
#include "hw/rtc.hh"
#include "hw/temp_dht.hh"
#include "input.hh"
#include <cstdio>
#include <pico/stdio.h>

int main() {
    stdio_init_all();

    RTCAdapter rtc;
    rtc.set_time(RTCAdapter::DEFAULT_DATETIME_2000_01_01);

    InputManager inputManager;
    TempSensorInput tempSensorInput;
    DisplayManager displayManager;
    App app_context_instance(displayManager, inputManager, tempSensorInput,
                             rtc);
    printf("# App established\n");

    displayManager.clear();

    while (true) {
        displayManager.clear(false);
        app_context_instance.dispatch();
        displayManager.commit();
        sleep_ms(TICK_RATE_MS);
    }

    return 0;
}
