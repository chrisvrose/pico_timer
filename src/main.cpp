#include <cstdio>
#include <pico/assert.h>
#include <pico/stdio.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <pico/types.h>
#include "app.hh"
#include "hw/rtc.hh"
#include "input.hh"
#include "hw/display.hh"
#include "hw/temp_dht.hh"
#include "config.hh"

extern "C"{
#include "pico/status_led.h"
}

bool led_state = true;

static const datetime_t DEFAULT_DATETIME_2000_01_01 = {
    .year  = 2000,
    .month = 1,
    .day   = 1,
    .dotw  = 6,   // 0=Sunday ... 6=Saturday
    .hour  = 0,
    .min   = 0,
    .sec   = 0,
};



int main(){
    stdio_init_all();

    if constexpr (WAIT_FOR_USB) {
        while (!stdio_usb_connected()) {
            sleep_ms(1000);
            printf(".\n");
        }
    }

    RTCAdapter rtc;
    rtc.set_time(DEFAULT_DATETIME_2000_01_01);
    bool ret = status_led_init();
    hard_assert(ret);

    InputManager inputManager;

    printf("# Input established\n");
    TempSensorInput tempSensorInput;
    printf("# DHT established\n");
    DisplayManager displayManager;
    printf("# Display established\n");
    App app_context_instance(displayManager,inputManager,tempSensorInput,rtc);
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
