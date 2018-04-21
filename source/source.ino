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
void wifi_connect();
void fileWrite();
String fileRead();
void fileOverwrite();
configure configRead();
void logFile();
void StrToInt();
sensor readSensor();
void bootSensor();
void bootOutput();
void output_control();
void printSensor();
void webserve();
void returnOK();
void returnFail();
void handleSlave();
void handleWater();
void handleStatus();
void handleConfig();
void handleForm();
void handleSDCard();
bool loadFromSdCard();

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(baud);
  Serial.setDebugOutput(debug);
  conf.isfirebase = firebase;
  bootSensor();
  bootOutput();
  wifi_connect();
  if(slave == 0 && SD.begin(SS)){
    hasSD = true;
    Serial.println("SD Card initialized.");
    conf = configRead("config.utc");
    webserve();
  }else{
    // Slave Mode
    Serial.println("Slave mode");
    HTTPClient http;
    http.begin(murl);
    int httpCode = http.GET();
    String payload = "";
    while(true){
      if (httpCode > 0) {
        // Get the request response payload
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode == HTTP_CODE_OK) {
          payload += http.getString();   //Get the request response payload
          Serial.println(payload);
          Serial.println("EOF");
          break;
        }
      }
      delay(500);
    }
    http.end();
    Serial.println(payload);
    StrToInt(payload);
  }
  
}

void loop(void) {
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
