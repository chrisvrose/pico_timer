#pragma once

#include <hardware/rtc.h>
#include <optional>
#include <pico/types.h>

/**
 * Abstracts away RTC Operations
 */
class RTCAdapter {
  private:
    bool is_initialized = false;

  public:
    RTCAdapter() { rtc_init(); }
    bool set_time(const datetime_t&);
    std::optional<datetime_t> get_time();
};
