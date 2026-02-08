
#include <pico/types.h>
enum CurrentMode{
    USUAL,
    SET_ALARM,
    SYNC_TIME
};
struct AppContext{
    datetime_t alarm_timer = {};
    char temperature[10] = {0};
    CurrentMode currentMode = SYNC_TIME;

    void dispatch();
    private:
    void dispatch_usual();
    void dispatch_sync();
    void transition(CurrentMode newState){
        this->currentMode = newState;
    }
};
extern AppContext app_context_instance;
