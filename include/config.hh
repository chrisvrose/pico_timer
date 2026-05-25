#pragma once
#include <cstdint>


/**
 * Whether to wait for USB to start the app or not.
 */
constexpr bool WAIT_FOR_USB = false;

/**
 * Each tick of I/O will be 100ms after finishing each frame.
 */
constexpr uint8_t TICK_RATE_MS = 100;
