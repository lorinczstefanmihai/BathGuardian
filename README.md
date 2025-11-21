| Supported Targets | ESP32 |
| ----------------- | ----- |

# 🛁 Bath Guardian – System Overview - IOT system to monitor the humidity, temperature and bad odour from bathroom.

<img width="735" height="487" alt="Image" src="https://github.com/user-attachments/assets/df97590a-e61e-4400-b2d7-9c8b7a48dbc0" />

The diagram illustrates the architecture of the Bath Guardian, an IoT-based monitoring and automation system built around an ESP32 microcontroller. The ESP32 operates in AP/STA mode, allowing it to connect to Wi-Fi networks while also hosting its own access point if needed.

##🔧 Key Components & Functionality
🌡️ Sensors (DHT11, MQ)

• DHT11: Measures temperature and humidity.
• MQ sensor: Detects gas levels (e.g., steam, VOCs), enhancing safety and environmental monitoring.
The ESP32 collects continuous sensor data for local processing and cloud reporting.

##🌈 RGB LED

• Provides real-time visual feedback (e.g., warnings, status indicators, or mode selections).

##🌐 MQTT Cloud Integration (HiveMQ)

• Sensor data and event updates are sent via Wi-Fi to an MQTT broker (HiveMQ).

• Enables remote monitoring, automation rules, dashboards, or mobile app integration.

##🔌 Relay Outputs (Fan & Light)

• The ESP32 controls fan and light relays to automate ventilation or illumination based on sensor readings or remote commands.

• Useful for humidity control, air quality, or safety response.

##🖥️ Local HTTP Server

• The ESP32 hosts an HTTP web interface accessible over Wi-Fi.

• Allows manual control of relays, real-time sensor monitoring, and system configuration directly from a browser.
