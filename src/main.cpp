#include <cstdio>
#include <optional>
#include <pico/assert.h>
#include <pico/stdio.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <pico/types.h>
#include "dht.h"
#include "hardware/rtc.h"
#include "context.hh"
#include "input.hh"

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

std::optional<std::pair<float,float>> measure_dht(dht_t dht){
    dht_start_measurement(&dht);
    sleep_ms(100);
    float humidity = 0,temp_c=0;
    dht_result_t res = dht_finish_measurement_blocking(&dht, &humidity,&temp_c);
    if(res==DHT_RESULT_OK){
        return std::optional<std::pair<float,float>>({humidity,temp_c});
    }else{
        printf("Print READOUT FAILED\n");
    }
    return std::optional<std::pair<float,float>>();

}

int main(){
    stdio_init_all();
    rtc_init();
    bool ret = status_led_init();
    hard_assert(ret);
    rtc_enable_alarm();
    InputManager inputManager;
    dht_t dht;
    dht_init(&dht,DHT11,pio0,17,true);
    AppContext app_context_instance;



    while(!stdio_usb_connected()){
        sleep_ms(1000);
        printf(".\n");
    }

    datetime_t date_mut = {};

    rtc_set_alarm(&update_alarm, update_alarm_callback);

    while (true) {
        std::optional<std::pair<float,float>> results = measure_dht(dht);
        const Input entered_input = inputManager.poll_input();
        std::pair<float,float> res_def = results.value_or({-1,-1});
        printf("Humidity: %f, Temp: %f ",res_def.first,res_def.second);
        app_context_instance.dispatch(entered_input);
        sleep_ms(10);
    }

    return 0;
}
