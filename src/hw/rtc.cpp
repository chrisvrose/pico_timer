#include <hardware/rtc.h>
#include <hw/rtc.hh>
#include <optional>
#include <pico/types.h>

bool RTCAdapter::set_time(const datetime_t& t) {
    bool was_set = rtc_set_datetime(&t);
    is_initialized |= was_set;
    return was_set;
}

std::optional<datetime_t> RTCAdapter::get_time() {
    if (is_initialized) {
        datetime_t t;
        bool is_running = rtc_get_datetime(&t);
        is_initialized &= is_running;
        if (is_running) {
            return std::make_optional(t);
        }
    }
    return std::nullopt;
}
