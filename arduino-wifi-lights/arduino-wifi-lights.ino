/*Includes*/
#include <WiFi.h>

/*Defines*/
#define SERIAL_SPEED 115200

#define LED_PIN 2

/*Global Variables*/
const char* ssid = "Liu Family";
const char* password = "1a2s3h4l5e6y7";

bool ledStatus = HIGH;

void setup() {
  Serial.begin(SERIAL_SPEED);
  
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  
}
