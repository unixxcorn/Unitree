#include "structure.h"
// ====================[ I/O Module ]====================
//  DHT         Temperature and Humidity sensor
//  YL-69       Soil moisture sensor
//  ZS-042      RTC Module
//  SDcard      Card Reader
//  Relay       Control Output
//  Mist Spray  Control Humidity
// ======================================================

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial.setDebugOutput(debug);
  bootSensor();
  bootOutput();
  wifi_connect();
  webserve();
  if(SD.begin(SS)){
    Serial.println("SD Card initialized.");
  }
  conf = configRead("config.utc");
}

void loop(void) {
  // put your main code here, to run repeatedly:
  sensor temp = readSensor();
  if(isnan(temp.humid) || isnan(temp.temp)){
    value.times = temp.times;
    value.mois = temp.mois;
  }else{
    value = temp;
  }
  output_control(value, conf);
  server.handleClient();
  delay(100);
}
