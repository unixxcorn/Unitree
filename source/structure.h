#include <ArduinoJson.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeLib.h>
#include "config.h"

DHT dht(dht_dpin, DHTTYPE); 
RtcDS3231<TwoWire> rtc(Wire);
ESP8266WebServer server(HTTPport);
typedef struct{
  float humid = 0.0;
  float temp = 0.0;
  unsigned int mois = 0;
  RtcDateTime times;
} sensor;
typedef struct{
  int isslave = 0;
  float humid = 0.0;
  float temp = 0.0;
  unsigned int mois = 0;
} configure;
String input = "";
File file;
configure conf;
sensor value;
bool hasSD = false;
