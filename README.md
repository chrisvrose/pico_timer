# Pico Timer

A small clock and temperature reading project using the Pi Pico.

Additionally, `timesync-sv` to sync time from a computer over serial.

Initial setup uses:
- SSD1306 Display (i2c)
- DHT11 temp+humidity sensor

## Setup

```bash
# After cloning
git submodule update --init --recursive

# Generate build files
cmake -B build

# Build it (finally)
cmake --build build -j`nproc`
```

### Timesync-server 

```
cd timesync-sv

cargo build --release

./target/release/timesync-sv
```

## Docs

Read more on getting started on the [documentation](./docs/index.md)
