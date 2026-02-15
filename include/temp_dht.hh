#pragma once
#include "dht.h"
#include <cstdint>
#include <optional>
struct TempHumidityMeasurement{
    float temp_in_c;
    float humidity_in_percentage;
};
class TempSensorInput {
    dht_t dht;
    std::optional<TempHumidityMeasurement> lastMeasurement;
    uint8_t cycleCount = 0;
    const uint8_t collectOn = 10;

    void try_poll();
    public:
    TempSensorInput();
    std::optional<TempHumidityMeasurement> try_poll_cached();

};
