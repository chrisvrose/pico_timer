# Pico Timer - Documentation

## Pages

- `index.md`: Main state machine + architecture overview
- `timesync.md`: USB time sync + host helper (`timesync-sv`)

## State Machine

The application operates on a state machine with three main states:

### State Diagram

```mermaid
stateDiagram-v2
    [*] --> SYNC_TIME

    SYNC_TIME --> USUAL: valid datetime
    SYNC_TIME --> SYNC_TIME: invalid input / parse fail

    USUAL --> SET_ALARM: PRESSED_BUTTON

    SET_ALARM --> USUAL: confirm
    SET_ALARM --> USUAL: cancel

    note right of SYNC_TIME: Display "Syncing Time"<br/>Poll serial/USB for YYYY-MM-DD HH:MM:SS<br/>Set RTC on success
    note right of USUAL: Display time + temperature + humidity<br/>Tick every TICK_RATE_MS<br/>Poll DHT via cached reads
    note right of SET_ALARM: Alarm config UI (future)<br/>Adjust via buttons (future)
```

### State Details

#### SYNC_TIME
- **Purpose:** Initialize the RTC with the correct current time
- **Entry:** On boot (initial state)
- **Exit Condition:** Valid datetime received and set in RTC
- **Display:** "Syncing Time"
- **Input:** Serial/USB input in format `YYYY-MM-DD HH:MM:SS`
- **Error Handling:** Invalid format → display error, retry

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
- **Input:** Button presses to adjust hour/minute (future)

### Transitions

| From | To | Trigger | Condition |
|------|-----|---------|-----------|
| SYNC_TIME | USUAL | Valid datetime input | RTC successfully set |
| SYNC_TIME | SYNC_TIME | Invalid input | Parse failed, retry |
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
  - Speed: 50kHz

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

## Next Steps

- [ ] Implement `InputManager::poll_time_from_stdio()` for time sync
- [ ] Implement button input polling
- [ ] Implement SET_ALARM state handler
- [ ] Add alarm trigger action
