# USB time sync (CDC serial)

Minimal CDC-serial time sync between the Pico firmware and a host helper (`timesync-sv`).

## Device behavior (current)

- Starts in `SYNC_TIME` and requests time over USB serial.
- Sends `REQ_TIME`, waits ~1s for a reply, and parses `ACK(...)`.
- Emits `ACK_TIME`, `ERR_TIME`, or `ERR_TMO` based on parse/timeout.
- On boot sets a safe default RTC value (`2000-01-01`) to avoid invalid reads.

Relevant code:
- `src/app.cpp`
- `src/input.cpp`

## Host helper (`timesync-sv`)

- Scans serial ports, selects the Pico by VID/PID.
- On `REQ_TIME`, replies with local time in the expected format.

Relevant code:
- `timesync-sv/src/main.rs`

## USB identification

- `USBD_VID=0xCA1F`
- `USBD_PID=0x51e9`

(Defined in `CMakeLists.txt`.)

## Protocol (current)

### Device → Host

- `REQ_TIME`
- `ACK_TIME`
- `ERR_TIME`
- `ERR_TMO`
- `# ...` debug lines

### Host → Device

- `ACK(YYYY-MM-DDTHH:MM:SS)\n`

Parsed on-device with:

- `sscanf("ACK(%hd-%hhd-%hhdT%hhd:%hhd:%hhd)", ...)`

Fields: `year`, `month`, `day`, `hour`, `min`, `sec`.
