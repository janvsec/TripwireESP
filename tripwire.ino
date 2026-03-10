// ##################################################
// INCLUDE LIBRARIES AND DEFINE PINS
#include <WiFi.h>
#include <WebServer.h>
#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 19
// ##################################################
// ENTER CORRECT WIFI NAME AND PASSWORD
const char* WIFI_SSID = "wifiname";
const char* WIFI_PASS = "wifipassword";
WebServer server(80);
// ##################################################
long distance = 0;
bool intruder = false;
bool lastIntruderState = false;
unsigned long intruderHoldMillis = 1000; 
unsigned long intruderTimestamp = 0;
// ##################################################
// READ DISTANCE FROM HC-SR04
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}
// ##################################################
// HANDLE API REQUEST AND RETURN JSON STATE
void handleAPI() {
  String json = "{";
  json += "\"intruder\":" + String(intruder ? "true" : "false") + ",";
  json += "\"distance\":" + String(distance);
  json += "}";
  server.send(200, "application/json", json);
}

// ##################################################
// SETUP WIFI: CONNECT OR START AP
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi: " + String(WIFI_SSID));
    Serial.print("Mode: STA\nIP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("API URL: http://" + WiFi.localIP().toString() + ":80/status");
  } else {
    Serial.println("WiFi not available, starting AP...");
    WiFi.softAP("ESP32_TripWire", "12345678");
    IPAddress ip = WiFi.softAPIP();
    Serial.println("Mode: AP\nSSID: ESP32_TripWire\nPassword: 12345678");
    Serial.print("API URL: http://" + ip.toString() + ":80/status");
  }
}
// ##################################################
// SETUP FUNCTION
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setupWiFi();

  server.on("/status", handleAPI);
  server.begin();
  Serial.println("\nAPI server started at /status");
}
// ##################################################
// MAIN LOOP: CHECK DISTANCE, BUZZER, AND INTRUDER STATE
void loop() {
  server.handleClient();
  long newDistance = readDistanceCM();
  distance = newDistance;
  bool detected = (distance > 0 && distance < 50);
  if (detected) {
    digitalWrite(BUZZER_PIN, HIGH);
    intruder = true;
    intruderTimestamp = millis();
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    if (millis() - intruderTimestamp > intruderHoldMillis) {
      intruder = false; 
    }
  }
  if (intruder != lastIntruderState || intruder) {
    lastIntruderState = intruder;
  }
  delay(100);
}
// #######################################################

