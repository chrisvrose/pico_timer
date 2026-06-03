#pragma once

#include <optional>
#include <pico/types.h>
enum Input{
    NO_INPUT,
    PRESSED_BUTTON,
    PRESSED_SECONDARY_BUTTON
};

enum COM_REQ_MESSAGE_TYPES{
    REQ_TIME,
    ACK_TIME,
    ERR_TIME,
    /** Timeout */
    ERR_TMO
};

class InputManager{
    private:
        void send_message(COM_REQ_MESSAGE_TYPES msg_type) const;
        std::optional<datetime_t> try_parse_req_time_response() const;
    public:
    inline Input poll_input() const{
        return NO_INPUT;
    }

    std::optional<datetime_t> request_time_from_com() const;
};
