# Arduino Smart Animal Feeder

An Arduino Uno prototype for dispensing dry food using a servo-operated gate. It supports timed feeding, a manual button and commands through the USB Serial Monitor.

## Project provenance

This repository is a **new reconstruction**, prepared in November 2026 from my description of an earlier Arduino animal feeder project. The original sketch, circuit and hardware details were not available. The Uno, servo mechanism, pin assignments and timing values below are assumptions for this reconstruction, not recovered specifications. This code and diagram should not be presented as the original university submission.

## Features

- Automatic dispensing every six hours while powered
- Manual feed using a button or the `f` Serial command
- Servo gate closes after 700 milliseconds without blocking the loop
- Button debouncing and a one-minute cooldown between feed starts
- LED feedback and a Serial status command
- Separate C++ controller logic with host-based automated checks

## Components

| Component | Quantity | Purpose |
| --- | --- | --- |
| Arduino Uno R3 | 1 | Controller |
| Small positional servo, such as SG90 | 1 | Operates an unloaded prototype gate |
| Normally open push button | 1 | Manual dispensing |
| Regulated 5 V servo supply | 1 | Must meet the chosen servo's stall current |
| Breadboard and jumper wires | As needed | Connections |
| USB cable | 1 | Uno power, programming and Serial Monitor |
| Hopper and lightweight gate | 1 | Mechanical prototype, not designed in this repository |

The servo is a gate actuator, not a continuous rotation motor. Food mass is not measured. The angle and opening time need calibration with the actual mechanism.

## Wiring

![Proposed wiring](docs/wiring.svg)

| Connection | Destination |
| --- | --- |
| Uno D9 | Servo signal |
| Uno D2 | One button terminal |
| Other button terminal | Uno GND |
| Servo positive | External regulated +5 V |
| Servo ground | External supply negative |
| Uno GND | External supply negative, shared ground |
| Uno USB | Computer |

`INPUT_PULLUP` is enabled, so the button connects D2 to ground when pressed. No external pull-up resistor is required. For a four-leg tactile switch, use terminals that are disconnected until pressed; same-side legs may already be internally connected.

Power the Uno by USB and the servo from the separate supply. Do not connect the external +5 V to the Uno 5 V pin in this arrangement. Disconnect power before changing wiring.

## Upload

1. Install Arduino IDE and the official Servo library through Library Manager if it is missing.
2. Open `firmware/SmartAnimalFeeder/SmartAnimalFeeder.ino`. Keep `FeederController.h` beside it.
3. Select Arduino Uno and the correct USB port.
4. Compile and upload, then open Serial Monitor at **9600 baud**.
5. Send `s` for status or `f` for a manual feed.

The first automatic feed happens six hours after startup. A manual feed restarts that interval. Restarting the Uno loses the schedule and count; it does not immediately dispense. For a short bench demonstration, temporarily set `FEED_INTERVAL_MS` to `10000UL` and `COOLDOWN_MS` to `5000UL`, then restore the normal values afterwards.

## Tests and verification

The pure C++ state controller was compiled with GCC using C++11 and warnings treated as errors:

```bash
mkdir -p build
g++ -std=c++11 -Wall -Wextra -Werror tests/controller_test.cpp -o build/controller_test
./build/controller_test
```

The included assertions passed for startup, scheduled feeding, gate closure, overlap prevention, manual feeding, cooldown, resetting the interval and unsigned timer rollover. These are new reconstruction tests, not evidence of testing carried out on the original project.

The complete Arduino sketch has **not** been compiled with the Arduino toolchain or tested on physical hardware here. Button wiring, servo response, power stability and dispensing accuracy need bench testing. See [the bench test checklist](docs/BENCH_TESTS.md).

## Limitations and next steps

This version has no Wi-Fi, app, cloud connection, food-level sensor, jam detection or real-time clock. USB Serial is its only external control interface. Possible extensions include an ESP32-based network interface, RTC-backed schedules, weighing portions and a low-food alert. These extensions are not implemented.

Use this as a supervised prototype, not the sole means of feeding an animal. Calibrate portions and verify gate closure before any practical use.
