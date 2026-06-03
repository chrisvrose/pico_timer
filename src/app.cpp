#include "app.hh"
#include <cstdio>
#include <hardware/rtc.h>
#include <optional>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/util/datetime.h>
#include <string>
#include <sys/unistd.h>

constexpr uint8_t datebuf_len = 48;
char datebuf[datebuf_len] = {0};

const char NO_DATE[] = "No date :(";
/**
 * Main app loop
 * This is represented as a standard input.
 * Input -> (state->state) -> output
 */
void App::dispatch() {
    Input enteredInput = inputManager.poll_input();
    auto env_input = tempSensorInput.try_poll_cached();

    switch (this->currentMode) {
    case USUAL:
        dispatch_usual(enteredInput, env_input);
        break;
    case SET_ALARM:
        break;
    case SYNC_TIME:
        dispatch_sync(enteredInput);
        break;
    case DEGRADED_NO_TIME:
        dispatch_degraded_no_time(enteredInput, env_input);
        break;
    }
    this->ticks_in_state++;
}
void App::dispatch_usual(
    Input entered_input,
    const std::optional<TempHumidityMeasurement> &env_input) {
    const std::optional<datetime_t> date_opt = rtcAdapter.get_time();
    hard_assert(date_opt.has_value());

    const datetime_t date = date_opt.value();
    memset(datebuf, 0, sizeof(datebuf));
    datetime_to_str(datebuf, sizeof(datebuf), &date);
    displayManager.drawTextWrapped(datebuf);

    draw_temp_humidity(env_input);
}

void App::dispatch_degraded_no_time(
    Input entered_input,
    const std::optional<TempHumidityMeasurement> &env_input) {

    displayManager.drawTextWrapped(NO_DATE);
    draw_temp_humidity(env_input);
}

void App::draw_temp_humidity(
    const std::optional<TempHumidityMeasurement> &env_input) const {
    char envText[17] = {0};

    float temp = env_input.has_value() ? env_input->temp_in_c : -1;
    snprintf(envText, 17, "T: %.2f 'C", temp);
    displayManager.drawTextWrapped(envText, 0, 24);

    float humidity =
        env_input.has_value() ? env_input->humidity_in_percentage : -1;
    snprintf(envText, 17, "H: %.2f %%", humidity);
    displayManager.drawTextWrapped(envText, 0, 32);
}

void App::dispatch_sync(Input entered_input) {
    // printf("We will pretend to sync time\n");
    displayManager.drawText("Syncing Time");

    char left_ticks_text[32] = {};

    snprintf(left_ticks_text, 32, "TTS: %d", 20 - ticks_in_state);

    displayManager.drawTextWrapped(left_ticks_text, 0, 32);
    std::optional<datetime_t> init_date = inputManager.request_time_from_com();

    if (init_date.has_value()) {
        printf("# Successfully polled data");
        const datetime_t init_date_val = init_date.value();
        bool was_set = rtcAdapter.set_time(init_date_val);

        if (was_set) {
            this->transition(CurrentMode::USUAL);
        } else {
            printf("# Could not set the time\n");
        }
    }
    if (ticks_in_state > 20) {
        this->transition(DEGRADED_NO_TIME);
    }
    sleep_us(20);
}
