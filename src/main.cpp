#include "app.hh"
#include "config.hh"
#include "hw/display.hh"
#include "hw/rtc.hh"
#include "hw/temp_dht.hh"
#include "input.hh"
#include <cstdio>
#include <pico/assert.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <pico/types.h>

static const datetime_t DEFAULT_DATETIME_2000_01_01 = {
    .year = 2000,
    .month = 1,
    .day = 1,
    .dotw = 6, // 0=Sunday ... 6=Saturday
    .hour = 0,
    .min = 0,
    .sec = 0,
};

int main() {
    stdio_init_all();

    RTCAdapter rtc;
    rtc.set_time(DEFAULT_DATETIME_2000_01_01);

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
