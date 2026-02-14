#include <cstdio>
#include <hardware/rtc.h>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/util/datetime.h>
#include "context.hh"

AppContext app_context_instance;

char datebuf[256]={0};
inline void print_date(){
    datetime_t date_mut;
    rtc_get_datetime(&date_mut);
    datetime_to_str(datebuf,sizeof(datebuf),&date_mut);
    printf("Date: %s\r",datebuf);
}
void AppContext::dispatch(Input enteredInput){
    switch (this->currentMode) {
    case USUAL:
        dispatch_usual(enteredInput);
    break;
    case SET_ALARM:
    break;
    case SYNC_TIME:
        dispatch_sync(enteredInput);
      break;
    }
}
void AppContext::dispatch_usual(Input entered_input){
    print_date();
}
void AppContext::dispatch_sync(Input entered_input){
    sleep_ms(1000);
    printf("We will pretend to sync time\n");
    datetime_t init_date = {
        .year=2026,
        .month=02,
        .day=010,
        .hour=13,
        .min=23,
        .sec=58
    };
    rtc_set_datetime(&init_date);
    sleep_us(64);
    this->transition(CurrentMode::USUAL);
}
