#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeLib.h>
#include "config.h"
// ====================[ I/O Module ]====================
//  DHT         Temperature and Humidity sensor
//  YL-69       Soil moisture sensor
//  ZS-042      RTC Module
//  SDcard      Card Reader
//  Relay       Control Output
//  Mist Spray  Control Humidity
// ======================================================

// Structure
DHT dht(dht_dpin, DHTTYPE); 
RtcDS3231<TwoWire> rtc(Wire);
WiFiServer server(HTTPport);
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
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", ((int)timezone)*3600, 60000);

// Define function
void bootSensor();
void bootOutput();
int bootSDcard();
void fileWrite(String filename, String data);
String fileRead(String filename);
void output_control(sensor value, configure conf);
void printSensor(sensor value);
void wlan_connect();
void web();
sensor readSensor();
sensor value;
configure runServer(sensor value, configure conf);
configure conf;

// Main Function
void setup(){
  Serial.begin(Baud);
  Serial.println("Booting..");
  bootSensor();
  wlan_connect();
  bootOutput();
  bootSDcard();
  conf = configRead("CONFIG.NTR");
  Serial.println(conf.isslave);
  Serial.println(conf.humid);
  Serial.println(conf.temp);
  Serial.println(conf.mois);
  NTP2RTC();
  Serial.println(ntp());
  printSensor(readSensor());
  delay(500);
}

void loop(){
  sensor temp = readSensor();
  if(isnan(temp.humid) || isnan(temp.temp)){
    value.times = temp.times;
    value.mois = temp.mois;
  }else{
    value = temp;
  }
  conf = runServer(value, conf);
  output_control(value, conf);
  /*if(digitalRead(dbpin)){
    printSensor(value);
    delay(5000);
  } // comment if don't want to see sensor value*/
  delay(500);
}
