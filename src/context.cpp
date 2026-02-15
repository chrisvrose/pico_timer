#include <cstdio>
#include <hardware/rtc.h>
#include <optional>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/util/datetime.h>
#include "context.hh"
#include "display.hh"
#include "temp_dht.hh"


char datebuf[256]={0};
inline void print_date(DisplayManager& displayManager){

}
void AppContext::dispatch(Input enteredInput, std::optional<TempHumidityMeasurement>& env_input){
    switch (this->currentMode) {
    case USUAL:
        dispatch_usual(enteredInput, env_input);
    break;
    case SET_ALARM:
    break;
    case SYNC_TIME:
        dispatch_sync(enteredInput);
      break;
    }
}
void AppContext::dispatch_usual(Input entered_input, std::optional<TempHumidityMeasurement>& env_input){
    datetime_t date_mut;
    rtc_get_datetime(&date_mut);
    datetime_to_str(datebuf,sizeof(datebuf),&date_mut);
    displayManager.drawTextWrapped(datebuf);

    char envText[17]={0};
    float temp = env_input.has_value()?env_input->temp_in_c:-1;
    snprintf(envText, 17, "T: %.2f 'C",temp);
    displayManager.drawTextWrapped(envText,0,24);

    float humidity = env_input.has_value()?env_input->humidity_in_percentage:-1;
    snprintf(envText, 17, "H: %.2f %%",humidity);
    displayManager.drawTextWrapped(envText,0,32);

}
void AppContext::dispatch_sync(Input entered_input){
    // printf("We will pretend to sync time\n");
    displayManager.drawText("Syncing");
    sleep_ms(1000);
    datetime_t init_date = {
        .year=2026,
        .month=02,
        .day=010,
        .hour=13,
        .min=23,
        .sec=58
    };
    rtc_set_datetime(&init_date);
    sleep_us(1000);
    this->transition(CurrentMode::USUAL);
}
