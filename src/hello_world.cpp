#include <pico/assert.h>
#include <pico/stdio.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <pico/types.h>
#include "hardware/rtc.h"
#include "context.hh"
extern "C"{
#include "pico/status_led.h"
}

bool led_state = true;

#define WAIT_FOR_USB

const datetime_t update_alarm = {
    .year=-1,
    .month=-1,
    .day=-1,
    .dotw=-1,
    .hour=-1,
    .min=-1,
    .sec=00,
};

void update_alarm_callback(){
    status_led_set_state(led_state);
    led_state = !led_state;
}

int main(){
    stdio_init_all();
    rtc_init();
    bool ret = status_led_init();
    hard_assert(ret);
    rtc_enable_alarm();

    #ifdef WAIT_FOR_USB
    while(!stdio_usb_connected()){
        sleep_ms(1000);
    }
    #endif

    datetime_t date_mut = {};

    rtc_set_alarm(&update_alarm, update_alarm_callback);

    while (true) {
        app_context_instance.dispatch();
        sleep_ms(100);
    }

    return 0;
}
