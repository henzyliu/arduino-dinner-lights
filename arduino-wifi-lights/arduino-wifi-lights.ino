/*Includes*/
#include <WiFi.h>
#include "config.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/*Macros*/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

#define SERIAL_SPEED 115200

#define LED_PIN 2
#define EXTERNAL_LED_PIN 23

/*Global Variables*/
bool ledStatus = HIGH;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Subscribe dinnerTimeFeed = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/dinnertime");

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
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
    digitalWrite(EXTERNAL_LED_PIN, ledStatus);
    count++;
    delay(duration);
  }

  ledStatus = previousLedStatus;
}

void MQTT_connect() {
  int8_t ret;
  
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.println("Connecting to MQTT... ");
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 1 seconds...");
       mqtt.disconnect();
       delay(1000);  // wait 1 second
  }
  Serial.println("MQTT Connected!");
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledStatus);
  digitalWrite(EXTERNAL_LED_PIN, ledStatus);
  
  // delete old config
  WiFi.disconnect(true);

  delay(1000);

  WiFi.onEvent(onWiFiConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(onWiFiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(onWiFiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  mqtt.subscribe(&dinnerTimeFeed);
}

void loop() {
  MQTT_connect();

  delay(1000);

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &dinnerTimeFeed) {
      Serial.print(F("Got: "));
      char* value = (char*)dinnerTimeFeed.lastread;
      Serial.println(value);
      if(*value == '1'){
        flickerLights();
      }
    }
  }
}
