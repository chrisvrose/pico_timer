# USB time sync (CDC serial)

Minimal CDC-serial time sync between the Pico firmware and a host helper (`timesync-sv`).

## Device behavior (current)

- Starts in state `SYNC_TIME` and requests time over USB serial.
- Sends `REQ_TME`, waits ~1s for a reply, and parses `ACK(...)`.
- Emits `ACK_TIME`, `ERR_TIME`, or `ERR_TMO` based on parse/timeout.
- On boot sets a safe default RTC value (`2000-01-01`) to avoid invalid reads.

Relevant code:
- `src/app.cpp`
- `src/input.cpp`

## Host helper (`timesync-sv`)

- Scans serial ports, selects the Pico by VID/PID.
- On `REQ_TIME`, replies with local time in the expected format.

## USB identification

The default configuration (`CMakeLists.txt`) changes the VID so that `timesync-sv` can filter and find the device.

- `USBD_VID=0xCA1F`
- `USBD_PID=0x51e9`

## Protocol (current)

### Device → Host

- `REQ_TME\n` -> Please send a SET_TIME
- `ACK_SET\n` -> ACK previous SET_<TME|ALR>
- `ERR_SET\n` -> ACK previous SET_<TME|ALR>
- `ERR_TMO\n` -> timed out waiting for a response to REQ_TIME
- `# ...\n` debug lines

### Host → Device

- `SET_TME(YYYY-MM-DDTHH:MM:SS)\n`
- `SET_ALM(YYYY-MM-DDTHH:MM:SS)\n`
