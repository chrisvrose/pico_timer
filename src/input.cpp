#include <cstdio>
#include <input.hh>
#include <iostream>
#include <optional>
#include <pico/stdio.h>
#include <pico/time.h>
#include <sys/select.h>
#include <sys/unistd.h>
#include <vector>

const std::vector<std::string> MESSAGE_TYPE_STRINGS = {"REQ_TIME", "ACK_TIME",
                                                       "ERR_TIME", "ERR_TMO"};

std::optional<datetime_t> InputManager::try_parse_req_time_response() const {
    char c[49] = {0};
    auto t1 = make_timeout_time_ms(1000);
    datetime_t d = {

    };
    int read_chars = stdio_get_until(c, 49, t1);
    if (read_chars == PICO_ERROR_TIMEOUT) {
        send_message(ERR_TMO);
        return std::nullopt;
    }
    printf("# READ %d chars\n", read_chars);
    int readParams = sscanf(c, "ACK(%hd-%hhd-%hhdT%hhd:%hhd:%hhd)", &d.year,
                            &d.month, &d.day, &d.hour, &d.min, &d.sec);
    if (readParams != 6) {
        send_message(ERR_TIME);
        return std::nullopt;
    } else {
        send_message(ACK_TIME);
        return std::make_optional(d);
    }
}

void InputManager::send_message(COM_REQ_MESSAGE_TYPES msg_type) const {
    std::cout << MESSAGE_TYPE_STRINGS[msg_type] << std::endl;
}

std::optional<datetime_t> InputManager::request_time_from_com() const {
    send_message(REQ_TIME);
    return try_parse_req_time_response();
}
