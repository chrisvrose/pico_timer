#pragma once

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


};
