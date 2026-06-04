# Pico Timer - Documentation

## Pages

- `index.md`: Main state machine + architecture overview
- `timesync.md`: USB time sync protocol + host helper (`timesync-sv`)

## Goals

- [X] Initial time on serial
- [X] Initial temp and humidity on serial
- [X] Output on display
- [X] Fetch time
- [ ] Interactivity from timesync-sv
  - [X] Idling - timesync-sv
  - [ ] Set time from timesync-sv
  - [ ] Set alarm from timesync-sv
- [ ] pico_timer
  - [ ] Input bufferring
  - [ ] UI - Have a better UI
- [ ] Alarm
  - [ ] Set alarm
  - [ ] Sync alarm
  - [ ] Ring alarm
  - [ ] Snooze alarm
- [ ] pico_timer components expansion
  - [ ] Battery powered
  - [ ] Box


## State Machine

The application operates on a state machine with four main states:

### Clock States

#### SYNC_TIME
- **Purpose:** Initialize the RTC with the correct current time
- **Entry:** On boot (initial state)
- **Exit Condition:** Valid datetime received and set in RTC
- **Display:** "Syncing Time"

#### USUAL
- **Purpose:** Normal clock operation with environment monitoring
- **Entry:** After successful time sync or button action from SET_ALARM
- **Exit Condition:** User presses button (future implementation)
- **Display:**
  - Row 0: Current time (from RTC)
  - Row 3: Temperature (from DHT11)
  - Row 4: Humidity (from DHT11)
- **Polling:** DHT11 sensor read every 10 cycles (~1 second at 100ms tick rate)

#### SET_ALARM
- **Purpose:** Configure alarm time
- **Entry:** User presses button in USUAL mode
- **Exit Condition:** Alarm set OR user cancels
- **Display:** Alarm configuration UI (to be designed)
- **Input:** Buttons (future)

#### DEGRADED_NO_TIME
- **Purpose:** Show a safe UI when RTC time isn’t set
- **Entry:** Timeout while waiting in `SYNC_TIME`
- **Display:** "No date :(" (time placeholder)
- **Exit Condition:** Host time sync succeeds

### Transitions

| From | To | Trigger | Condition |
|------|-----|---------|-----------|
| SYNC_TIME | USUAL | Valid datetime input | RTC successfully set |
| SYNC_TIME | SYNC_TIME | Invalid input | Parse failed, retry |
| SYNC_TIME | DEGRADED_NO_TIME | Timeout | No time received |
| DEGRADED_NO_TIME | USUAL | Host time set | RTC successfully set |
| USUAL | SET_ALARM | Button pressed | PRESSED_BUTTON |
| SET_ALARM | USUAL | Alarm confirmed | Alarm saved |
| SET_ALARM | USUAL | User cancelled | No changes |

## Architecture

### Components

- **App**: Main state machine orchestrator (app.hh/cpp)
- **DisplayManager**: SSD1306 display control (hw/display.hh/cpp)
- **InputManager**: Button and serial input handling (input.hh/cpp)
- **TempSensorInput**: DHT11 sensor polling (hw/temp_dht.hh/cpp)

### Hardware Pins

- **I2C Display (SSD1306):**
  - SDA: GPIO 14
  - SCL: GPIO 15
  - Address: 0x3C

- **DHT11 Sensor:**
  - Data: GPIO 17
  - PIO: pio0

- **Status LED:**
  - Controlled by pico_status_led library
  - Blinks on each RTC alarm tick

### Configuration

See `include/config.hh` for compile-time configuration:
- `WAIT_FOR_USB`: Wait for USB connection before starting
- `TICK_RATE_MS`: Main loop tick rate (100ms default)
