#include "temp_dht.hh"
#include "dht.h"
#include <cstdio>
#include <optional>
#include <pico/time.h>

TempSensorInput::TempSensorInput(){
    dht_init(&this->dht, DHT11, pio0, 17,true);
    sleep_ms(25);
}

// 0->start measure ...     collectOn -> save measure and write out, set back to 0
void TempSensorInput::try_poll(){
    printf("DHT Cpoll %hhud\n",cycleCount);
    if(cycleCount==0){
        dht_start_measurement(&this->dht);
        cycleCount++;
    }else if(cycleCount==collectOn){
        float humidity = 0,temp_c=0;
        dht_result_t res = dht_finish_measurement_blocking(&dht, &humidity,&temp_c);
        if(res==DHT_RESULT_OK){
            lastMeasurement = std::make_optional(TempHumidityMeasurement {
                .temp_in_c=temp_c,
                .humidity_in_percentage=humidity
            });
        }else{
            printf("Print READOUT FAILED\n");
        }

        cycleCount = 0;
    }else{
        cycleCount++;
    }
}

std::optional<TempHumidityMeasurement> TempSensorInput::try_poll_cached(){
    this->try_poll();
    return lastMeasurement;
}
