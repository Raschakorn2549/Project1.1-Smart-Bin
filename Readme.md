# ♻️ Smart Bin — Automatic Metal / Non-metal Sorting

An ESP32-based bin that notices when trash is dropped in, checks whether it is metal,
and turns a servo to send it to the right compartment. Counts are published over **MQTT** in real time
so they can be shown on a dashboard.

<!-- TODO: ใส่รูปตัวเครื่องจริง / คลิปตอนแยกขยะ / ภาพวงจร -->
<!-- ![Smart bin](docs/smartbin.jpg) -->

## How it works

```
 Ultrasonic (HC-SR04) ── object < 15 cm? ──► Inductive sensor ── metal?
                                                   │
                                   yes ◄───────────┴──────────► no
                              servo → 0° (metal)          servo → 180° (non-metal)
                                                   │
                                     update counters, publish MQTT
                                     topic: trash/object
```

Example message:

```json
{"data": {"amount": 12, "non_metal": 7, "metal": 5}}
```

A push button resets all counters.

## Hardware

| Part | Purpose |
|---|---|
| ESP32 | Controller + Wi-Fi |
| HC-SR04 ultrasonic sensor | Detect an incoming object |
| Inductive proximity sensor | Metal / non-metal check |
| Servo motor | Tilt the flap to the correct side |
| Push button | Reset counters |

### Pin configuration

| Signal | Pin |
|---|---|
| Ultrasonic Trig | 9 |
| Ultrasonic Echo | 10 |
| Inductive sensor | 2 |
| Servo | 6 |
| Reset button | 12 |

## Tech stack

ESP32 · Arduino (C++) · MQTT (`broker.hivemq.com`) · PubSubClient · Servo

## Getting started

1. Install these libraries in Arduino IDE: **PubSubClient**, **Servo**
2. Create `secrets.h` next to `smartbinproject.ino` (it is git-ignored):

```cpp
#define WIFI_SSID      "your_wifi_name"
#define WIFI_PASSWORD  "your_wifi_password"
#define MQTT_CLIENT    "your_client_id"
#define MQTT_USERNAME  ""
#define MQTT_PASSWORD  ""
#define LINE_TOKEN     ""
```

3. Adjust the pins in `smartbinproject.ino` to match your wiring, then upload.
4. Subscribe to `trash/object` on the broker (e.g. with MQTT Explorer or Node-RED) to see the counts.

## Project structure

```
├── smartbinproject.ino   # Main firmware
├── secrets.h             # Wi-Fi / MQTT credentials (not committed)
└── Readme.md
```

## Future work

- Replace the inductive-only check with a camera + image classifier to sort plastic / paper / glass
- Dashboard showing fill level and daily statistics
