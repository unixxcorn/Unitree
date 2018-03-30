#include "structure.h"
// ====================[ I/O Module ]====================
//  DHT         Temperature and Humidity sensor
//  YL-69       Soil moisture sensor
//  ZS-042      RTC Module
//  SDcard      Card Reader
//  Relay       Control Output
//  Mist Spray  Control Humidity
// ======================================================
void webserve();

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial.setDebugOutput(debug);
  bootSensor();
  bootOutput();
  wifi_connect();
  if(slave == 0 && SD.begin(SS)){
    hasSD = true;
    Serial.println("SD Card initialized.");
    conf = configRead("config.utc");
    webserve();
  }/*else{ // Slave Mode Not Ready
    Serial.println("Slave mode");
    HTTPClient http;
    http.begin(murl);
    int httpCode = http.GET();
    Serial.println();
    Serial.println(http.getString());
    if (httpCode > 0) {
      // Get the request response payload
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      Serial.println((int)root["temp"]);
      
    }
    http.end();
  }*/
}

void loop(void) {
  // put your main code here, to run repeatedly:
  
  if(millis() % 1000 == 0){
    sensor temp = readSensor();
    if(isnan(temp.humid) || isnan(temp.temp)){
      value.times = temp.times;
      value.mois = temp.mois;
      printSensor(value);
    }else{
      value = temp;
    }
    output_control(value, conf);    
  }
  if(hasSD){
    server.handleClient();
    delay(100);
  }else{
    delay(300000);
  }
}
