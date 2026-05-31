# Pico Timer

A small clock and temperature reading project using the Pi Pico.

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

## Docs

Read more on getting started on the [documentation](./docs/index.md)
