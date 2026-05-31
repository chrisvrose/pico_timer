# Feature: USB time sync (CDC serial)

This repo implements a minimal **USB CDC serial time sync** loop between:

- the Pico firmware (device), and
- a host-side helper called `timesync-sv` (host “server”).

The goal is to set the Pico RTC without network hardware.

## Components

### Device (Pico firmware)

- The app starts in `SYNC_TIME`.
- In `SYNC_TIME`, the device asks the host for time over USB serial.

Relevant code:
- `src/app.cpp`: calls `InputManager::request_time_from_com()` while in `SYNC_TIME`.
- `src/input.cpp`: sends `REQ_TIME`, waits for `ACK(...)`, and emits status messages.

### Host helper (`timesync-sv`)

A simple “server/daemon” that:

- scans available serial ports
- selects the Pico by USB VID/PID
- reads lines from the Pico
- when it sees `REQ_TIME`, replies with current host time

Relevant code:
- `timesync-sv/src/main.rs`

## USB identification

The Pico is identified on the host by the USB VID/PID set in the firmware build:

- `USBD_VID=0xCA1F`
- `USBD_PID=0x51e9`

(See `CMakeLists.txt`.)

## Current protocol (as implemented today)

All messages are newline-delimited text lines.

### Device → Host

- `REQ_TIME`
  - request that the host send the current time

- `ACK_TIME`
  - **currently means:** the device parsed the host’s `ACK(...)` successfully
  - (note: it does *not* necessarily mean the RTC was set; see `src/app.cpp`)

- `ERR_TIME`
  - parsing failed (unexpected format)

- `ERR_TMO`
  - timed out waiting for the host response

- `# ...`
  - debug lines (e.g. `# READ <n> chars`)

### Host → Device

- `ACK(YYYY-MM-DDTHH:MM:SS)`

Parsed on-device via:

- `sscanf("ACK(%hd-%hhd-%hhdT%hhd:%hhd:%hhd)", ...)`

Fields:
- `year` (int16)
- `month` (int8)
- `day` (int8)
- `hour` (int8)
- `min` (int8)
- `sec` (int8)

## Timing / behavior notes

- Device waits ~1s for an `ACK(...)` response.
- `SYNC_TIME` currently requests time repeatedly (see `src/app.cpp` tick loop).
- If you want more stable behavior, throttle `REQ_TIME` (e.g. once per second).

## Next steps (later)

- Replace `ACK(...)` with a clearer command naming (e.g. `SET_TIME ...`).
- Make `ACK_TIME` reflect actual `rtc_set_datetime(...)` success.
- Add alarm commands.
