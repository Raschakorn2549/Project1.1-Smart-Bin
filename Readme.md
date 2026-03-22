# ♻️ Smart Trash Sorting System

ระบบถังขยะอัจฉริยะที่แยกประเภทขยะโลหะและไม่ใช่โลหะอัตโนมัติ พร้อมส่งข้อมูลผ่าน MQTT

## 📌 Features
- ตรวจจับวัตถุด้วย Ultrasonic Sensor
- แยกแยะโลหะ/ไม่ใช่โลหะด้วย Inductive Sensor
- หมุน Servo Motor เพื่อแยกขยะอัตโนมัติ
- ส่งข้อมูลจำนวนขยะผ่าน MQTT แบบ Real-time
- รีเซ็ตข้อมูลได้ด้วยปุ่ม

## 🛠️ Hardware
- ESP32
- Ultrasonic Sensor (HC-SR04)
- Inductive Sensor
- Servo Motor
- Push Button

## 📡 Tech Stack
- **Microcontroller**: ESP32
- **Protocol**: MQTT (broker.hivemq.com)
- **Notification**: LINE Notify
- **Library**: WiFi, PubSubClient, Servo

## 📁 Project Structure
```
├── main.ino          # โค้ดหลัก
├── secrets.h         # WiFi, MQTT, LINE credentials (ไม่อัป GitHub)
└── README.md
```

## ⚙️ Installation

1. ติดตั้ง Library ใน Arduino IDE:
   - `PubSubClient`
   - `Servo`

2. สร้างไฟล์ `secrets.h`:
```cpp
#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"
#define MQTT_CLIENT "your_client_id"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define LINE_TOKEN "your_line_token"
```

3. แก้ Pin ให้ตรงกับวงจรของคุณใน `main.ino`

## 🔌 Pin Configuration
| ชื่อ | Pin |
|---|---|
| Ultrasonic Trig | 9 |
| Ultrasonic Echo | 10 |
| Inductive Sensor | 2 |
| Servo Motor | 6 |
| Reset Button | 12 |

## 🚀 How it works
1. Ultrasonic sensor ตรวจจับวัตถุในระยะ 15 cm
2. Inductive sensor ตรวจสอบว่าเป็นโลหะหรือไม่
3. Servo หมุนไปทางซ้าย (โลหะ) หรือขวา (ไม่ใช่โลหะ)
4. ส่งข้อมูลจำนวนขยะไปยัง MQTT broker

## 🔐 Environment Variables
อย่าลืมสร้างไฟล์ `secrets.h` ไว้เก็รหัสตัวแปร