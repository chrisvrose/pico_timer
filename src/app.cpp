#include <cstdio>
#include <hardware/rtc.h>
#include <optional>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/util/datetime.h>
#include <string>
#include "app.hh"

char datebuf[64]={0};


/**
 * Main app loop
 * This is represented as a standard input.
 * Input -> (state->state) -> output
 */
void App::dispatch(std::optional<TempHumidityMeasurement>& env_input){
    Input enteredInput = inputManager.poll_input();

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
void App::dispatch_usual(Input entered_input, std::optional<TempHumidityMeasurement>& env_input){
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

void App::dispatch_sync(Input entered_input){
    // printf("We will pretend to sync time\n");
    displayManager.drawText("Syncing Time");
    std::optional<datetime_t> init_date = inputManager.poll_time_from_stdio();

    if(init_date.has_value()){
        printf("Has successfully polled data");
        rtc_set_datetime(&init_date.value());
        this->transition(CurrentMode::USUAL);
    }
    // rtc_set_datetime(&init_date);
    sleep_us(1000);
}
