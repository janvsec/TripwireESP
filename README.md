# TripwireESP
**ESP32 Tripwire Monitoring System with Web API**

This project is an ESP32-based smart tripwire system that detects objects using an HC-SR04 ultrasonic sensor. When something comes within 50 cm, a buzzer sounds and the system updates its state via a web API, allowing remote monitoring. It automatically connects to a Wi-Fi network or creates its own access point, making it ready for integration with automation platforms or notifications.

## Wiring Diagram
![Wiring Diagram](images/wiring.png)

## Pinout

| Component       | ESP32 Pin | Notes                     |
|-----------------|-----------|---------------------------|
| **HC-SR04 VCC** | 3.3V      | Power supply             |
| **HC-SR04 Trig**| GPIO 5    | Trigger signal           |
| **HC-SR04 Echo**| GPIO 18   | Echo signal              |
| **HC-SR04 GND** | GND       | Ground                   |
| **Buzzer GND**  | GND       | Ground                   |
| **Buzzer VCC**  | GPIO 19   | Output signal            |

## Features
- Real-time distance detection with HC-SR04  
- Audible alert via buzzer when object is within 50 cm  
- Web API endpoint `/status` returns JSON with distance and intruder state  
- Automatic Wi-Fi connection or AP fallback  
- Serial output shows detailed status, IP, and API URL for easy monitoring  

## Usage

1. Upload the code to your ESP32.  
2. Connect HC-SR04 and buzzer according to the pinout.  
3. Power on the ESP32.  
4. Check Serial Monitor for Wi-Fi or AP connection details.
5. Access the API from any device on the same network.

## Example output
```
Connected to WiFi: Free_wifi1
Mode: STA
IP Address: 192.168.1.55
API URL: http://192.168.1.55:80/status
API server started at /status
```

Or if Wi-Fi is unavailable:

```
WiFi not available, starting AP...
Mode: AP
SSID: ESP32_TripWire
Password: 12345678
API URL: http://192.168.4.1:80/status
API server started at /status
```


GET ``` http://<ESP32_IP>:80/status```

Example JSON response:

```json
{
  "intruder": true,
  "distance": 35
}
```
```json
"intruder": true if an object is detected within 50 cm, otherwise false
"distance": latest measured distance in centimeters
```
This API allows integration with external systems like N8N, email notifications, or custom dashboards.
