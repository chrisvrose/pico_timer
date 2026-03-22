#pragma once

#include <optional>
#include <pico/types.h>
enum Input{
    NO_INPUT,
    PRESSED_BUTTON,
    PRESSED_SECONDARY_BUTTON
};

class InputManager{
    public:
    inline Input poll_input(){
        return NO_INPUT;
    }

    std::optional<datetime_t> poll_time_from_stdio();
};
