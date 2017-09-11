/*
  Simple web server for ESP8266 Node MCU.
  The MAC adress for my unit seems to be:
    5c:cf:7f:a3:13:8b
  That might come in handy..
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../aREST/aREST.h"

#define LED D0
#define DEBUG_FLAG 1

// Constants
const int16_t BLINK_TIME = 100;
const int16_t INIT_WAIT = 100;
const int16_t RECHECK_TIME = 500;
// const char* SSID = "Apartment Sweden 1404";
// const char* PASSW = "snabbit99";
const char* SSID = "Tage";
const char* PASSW = "W1llYs747AB";

aREST rest = aREST();

WiFiServer server(80);

int data_s;
int no_of_times;
int key;

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

int ledCtrl(String arg) {
  flashLed(LED, arg.toInt());
  return 1;
}

int checkKey(String arg) {

}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(INIT_WAIT);

  data_s = 0;
  no_of_times = 5;
  rest.variable((char* )"data_s", &data_s);
  rest.variable((char* )"no_of_times", &no_of_times);

  rest.function((char *)"ledCtrl", ledCtrl);

  rest.set_id((char *)"1");
  rest.set_name((char *)"nodeMCU");

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
}

void loop(void) {
  // server.handleClient();
  WiFiClient client = server.available();
  if (!client)
    return;

  while(!client.available())
    delay(1);

  rest.handle(client);
}
