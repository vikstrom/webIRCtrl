/*
  Simple web server for ESP8266 Node MCU.
  The MAC adress for my unit seems to be:
    5c:cf:7f:a3:13:8b
  That might come in handy..
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../aREST/aREST.h"

#define LED D0
#define DEBUG_FLAG 1

// Constants
const int16_t BLINK_TIME = 100;
const int16_t INIT_WAIT = 100;
const int16_t RECHECK_TIME = 500;
const char* SSID = "Appartment Sweden 1404";
const char* PASSW = "snabbit99";

aREST rest = aREST();

ESP8266WebServer server(80);

int data_s;
int no_of_times

void debugPrint(const char* d_str) {
  if (DEBUG_FLAG) {
    Serial.println(d_str);
  }
  else return;
}

void flashLed(int16_t led_pin, int16_t repeat = 2) {
  const int16_t flash_time = 100;
  for (int16_t i = 0; i < repeat; i++) {
    digitalWrite(led_pin, 1);
    delay(flash_time);
    digitalWrite(led_pin, 0);
    delay(flash_time);
  }
}

void ledCtrl(char* args) {

}

void handleRoot() {
  debugPrint("Sending 200 and text!");
  server.send(200, "text/plain", "Wazzup?!");
}

void handleLed() {
  debugPrint("Trying to flash LED");
  flashLed(LED, 5);
  server.send(200, "text/plain", "OK");
}

void intit_server() {
  debugPrint("intit_server");
  server.on("/", handleRoot);
  server.on("/flash", handleLed);
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(INIT_WAIT);

  rest.variable("data_s", &data_str)
  rest.variable("no_of_times" &no_of_times)

  rest.function("ledCtrl", ledCtrl)

  Serial.print("Connecting");
  WiFi.begin(SSID, PASSW);
  int16_t counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    flashLed(LED);
    Serial.print(".");
    delay(RECHECK_TIME);
    counter++;
    if (!(counter % 5)) Serial.println(WiFi.status());
  }

  Serial.println("Connected!");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

  intit_server();
}

void loop(void) {
  server.handleClient();
}
