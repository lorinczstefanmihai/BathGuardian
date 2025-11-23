# 🛁 Bath Guardian – AWS IoT Edition ( THIS IS THE PROTOTYPE)

An ESP32-powered monitoring & automation system for humidity, gas levels, temperature, and bathroom safety.

Bath Guardian uses an ESP32 microcontroller, environmental sensors, and AWS IoT Core to provide real-time monitoring, automation, and remote control. The system can operate both locally (via its built-in web server) and through the cloud.

## 🚀 System Architecture Overview
ESP32 (AP/STA Mode)
The ESP32 runs in dual AP/STA mode, allowing it to connect to your Wi-Fi network while also offering an access point for local setup or fallback access.

<img width="800" height="500" alt="Image" src="https://github.com/user-attachments/assets/58ba41c1-b94e-4eff-9a1b-04f9b07a71dc" />


## 🔧 Hardware Components
🌡️ Sensors

DHT11 – Temperature & humidity

MQ-series gas sensor – Detects gases/VOCs (steam, odors, poor air quality)

The ESP32 collects continuous sensor data and pushes it to the cloud.

## 🌈 RGB LED

Provides visual indicators for system status, warnings, or mode states.

🔌 Relay Outputs

Controls:

Ventilation fan

Lighting

Relays can be triggered automatically (based on sensor thresholds) or remotely via AWS IoT commands.

## ☁️ AWS Cloud Integration

Bath Guardian connects to the cloud using:

AWS IoT Core (MQTT)

The ESP32 publishes data to custom MQTT topics.
Example topic used:

test_topic/esp32

<img width="1894" height="889" alt="Image" src="https://github.com/user-attachments/assets/79c46600-4433-4142-a1b0-28637f12f564" />

AWS IoT Core receives these messages for monitoring, automation, and further processing.

## 🖥️ Local Web Interface

The ESP32 also hosts a local HTTP web server, accessible over Wi-Fi.

Features:

Live sensor reading display

Local configuration

This allows full control even without internet.

<img width="925" height="1861" alt="Image" src="https://github.com/user-attachments/assets/3ac3893e-1f8f-48cd-a653-edd1d62e1cc4" /># 🛁 Bath Guardian – AWS IoT Edition ( THIS IS THE PROTOTYPE)

An ESP32-powered monitoring & automation system for humidity, gas levels, temperature, and bathroom safety.

Bath Guardian uses an ESP32 microcontroller, environmental sensors, and AWS IoT Core to provide real-time monitoring, automation, and remote control. The system can operate both locally (via its built-in web server) and through the cloud.

## 🚀 System Architecture Overview
ESP32 (AP/STA Mode)
The ESP32 runs in dual AP/STA mode, allowing it to connect to your Wi-Fi network while also offering an access point for local setup or fallback access.

<img width="800" height="500" alt="Image" src="https://github.com/user-attachments/assets/58ba41c1-b94e-4eff-9a1b-04f9b07a71dc" />


## 🔧 Hardware Components
🌡️ Sensors

DHT11 – Temperature & humidity

MQ-series gas sensor – Detects gases/VOCs (steam, odors, poor air quality)

The ESP32 collects continuous sensor data and pushes it to the cloud.

## 🌈 RGB LED

Provides visual indicators for system status, warnings, or mode states.

🔌 Relay Outputs

Controls:

Ventilation fan

Lighting

Relays can be triggered automatically (based on sensor thresholds) or remotely via AWS IoT commands.

## ☁️ AWS Cloud Integration

Bath Guardian connects to the cloud using:

AWS IoT Core (MQTT)

The ESP32 publishes data to custom MQTT topics.
Example topic used:

test_topic/esp32

<img width="1894" height="889" alt="Image" src="https://github.com/user-attachments/assets/79c46600-4433-4142-a1b0-28637f12f564" />

AWS IoT Core receives these messages for monitoring, automation, and further processing.

## 🖥️ Local Web Interface

The ESP32 also hosts a local HTTP web server, accessible over Wi-Fi.

Features:

Live sensor reading display

Local configuration

This allows full control even without internet.

<img width="925" height="1861" alt="Image" src="https://github.com/user-attachments/assets/3ac3893e-1f8f-48cd-a653-edd1d62e1cc4" />
