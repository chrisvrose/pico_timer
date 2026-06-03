#pragma once

#include <hardware/rtc.h>
#include <optional>
#include <pico/types.h>

/**
 * Abstracts RTC Operations
 */
class RTCAdapter {
  private:
    bool is_initialized = false;

  public:
    static constexpr datetime_t DEFAULT_DATETIME_2000_01_01 = {
        .year = 2000,
        .month = 1,
        .day = 1,
        .dotw = 6, // 0=Sunday ... 6=Saturday
        .hour = 0,
        .min = 0,
        .sec = 0,
    };

    RTCAdapter() { rtc_init(); }

    bool set_time(const datetime_t &);
    std::optional<datetime_t> get_time();
};
