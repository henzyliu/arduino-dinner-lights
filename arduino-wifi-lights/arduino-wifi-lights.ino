/*Includes*/
#include <WiFi.h>

/*Defines*/
#define SERIAL_SPEED 115200

#define LED_PIN 2

#define PUSH_BUTTON 0

/*Global Variables*/
const char* ssid = "Liu Family";
const char* password = "1a2s3h4l5e6y7";

bool ledStatus = HIGH;

void onWiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void flickerLights() {
  uint8_t count = 0;
  uint8_t repititions = 20;
  int duration = 500;
  bool previousLedStatus = ledStatus;

  Serial.println("DINNER TIME!");
  
  while (count < repititions) {
    ledStatus = !ledStatus;
    digitalWrite(LED_PIN, ledStatus);
    count++;
    delay(duration);
  }

  ledStatus = previousLedStatus;
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);
  
  // delete old config
  WiFi.disconnect(true);

  delay(1000);

  WiFi.onEvent(onWiFiConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(onWiFiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(onWiFiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.begin(ssid, password);

  digitalWrite(LED_PIN, ledStatus);
}

void loop() {
  delay(500);
  if (digitalRead(PUSH_BUTTON) == LOW){
    flickerLights();
  }
}
