#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>

// ====================[ I/O Module ]====================
//  DHT         Temperature and Humidity sensor
//  YL-69       Soil moisture sensor
//  ZS-042      RTC Module
//  SDcard      Card Reader
//  Relay       Control Output
//  Mist Spray  Control Humidity
// ======================================================

// Config Humidity and Temperature Sensor (DHT11)
#define DHTTYPE DHT11
#define dht_dpin 2

// Config Moisture Sensor (YL-69)
#define yl_apin 0

// Config Wireless Lan
#define ssid "U_f001"     //Wifi name
#define password "192.168.1.1" //Wifi password

// Config SD card
#define CSpin 15
// Config Relay pin
#define relaypin 0

// Config Debug pin
#define dbpin 16
#define Baud 115200

// Config Port
#define HTTPport 80

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
  float humid = 50.0;
  float temp = 50.0;
  unsigned int mois = 80;
} configure;
String input = "";
File file;

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
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
  Serial.begin(Baud);
  Serial.println("Booting..");
  bootSensor();
  wlan_connect();
  bootOutput();
  bootSDcard();
  conf = configRead("CONFIG.NTR");
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
  if(digitalRead(dbpin)){
    printSensor(value);
    delay(5000);
  }
  delay(500);
}

// Function

void bootSensor(){
  Serial.println("Checking Sensor"); 
  dht.begin();  // start DHT Sensor
  rtc.Begin();  // start RTC (using I2C)
  pinMode(dbpin, INPUT); // Debug Pin
  delay(300);
}

void bootOutput(){
  Serial.println("Checking Output");
  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin, 1);
  delay(300);
  digitalWrite(relaypin, 0);
}

int bootSDcard(){
  Serial.print("Accessing SD card...");
  if(!SD.begin(CSpin)){
    Serial.println("Fail to access!");
    return 1;
  }
  Serial.println("SD card is good.");
  Serial.println("Reading configuration from SD Card");
  
  return 0;
}

sensor readSensor(){
  sensor value;
  value.times = rtc.GetDateTime();
  value.humid = dht.readHumidity();
  value.temp  = dht.readTemperature();  
  value.mois  = ((1024 - analogRead(yl_apin))/10);
  return value;
}

void printSensor(sensor value){
  Serial.print("[");
  Serial.print(value.times.Day(), DEC);
  Serial.print("/");
  Serial.print(value.times.Month(), DEC);
  Serial.print("/");
  Serial.print(value.times.Year(), DEC);
  Serial.print(" - ");
  Serial.print(value.times.Hour(), DEC);
  Serial.print(":");
  Serial.print(value.times.Minute(), DEC);
  Serial.print(":");
  Serial.print(value.times.Second(), DEC);
  Serial.print("] ");
  Serial.println("");
  
  Serial.print("Humid : ");
  Serial.print(value.humid, 2);
  Serial.println(" %");
  
  Serial.print("Temp  : ");
  Serial.print(value.temp, 2);
  Serial.println(" C");
  
  Serial.print("Mois  : ");
  Serial.print(value.mois, DEC);
  Serial.println("");
}

void output_control(sensor value, configure conf){
  if(value.mois < conf.mois){
    digitalWrite(relaypin, 1);
  }
  /*if(value.humid > conf.humid || value.temp > conf.temp || value.mois < conf.mois){
    digitalWrite(relaypin, 1);
  }else{
    digitalWrite(relaypin, 0);
  }*/
  
}

void wlan_connect(){
  Serial.println("Connecting to WiFi"); 
  WiFi.begin(ssid, password);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print("|");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void fileWrite(String filename, String data){
  file = SD.open(filename, FILE_WRITE);
  file.println(data);
  file.close();
}

String fileRead(String filename){
  file = SD.open(filename);
  if(file){
    String fileRead = "";
    while (file.available()) {
      fileRead += (char)file.read();
    }
    file.close();
    return fileRead;
  }
  else{
    return "error";
  }
}

configure configRead(String fileName){
  if(fileRead(fileName) != "error"){
    String confi = fileRead(fileName);
    Serial.println(confi);
    conf.isslave = (int)confi[0];
    conf.humid = (float)confi[1];
    conf.temp = (float)confi[2];
    conf.mois = (int)confi[3];
    Serial.println((int)confi[0]);
    Serial.println((float)confi[1]);
    Serial.println((float)confi[2]);
    Serial.println((int)confi[3]);
  }else{
    file = SD.open(fileName, FILE_WRITE);
    file.printf("%c%c%c%c", 65 , 65 , 65 , 65);
    file.close();
    String confi = fileRead(fileName);
    Serial.println(confi);
    Serial.println((int)confi[0]);
    Serial.println((float)confi[1]);
    Serial.println((float)confi[2]);
    Serial.println((int)confi[3]);
  }
}
  
configure runServer(sensor value, configure conf){
  WiFiClient client = server.available();
  if(client){ 
      if(client.available()){
        String request = client.readStringUntil('\r');
        Serial.println(request);
        String filename = "";
        int i = 5;
        if(request[0] == 'G' || request[0] == 'g'){
          while(request[i] != ' ' && request[i] != '?'){
            filename += request[i];
            i++;
          }
          if(i == 5){
            filename = "index";
          }
          if(filename != "error" && filename != "favicon.ico" && filename != "status" && filename != "config"){
            filename += ".NTR";
            Serial.println(filename);
            String page = fileRead(filename);
            response(page, client);
          }else if(filename == "status"){
            String page = "<!DOCTYPE HTML> <html> <head> <meta http-equiv=\"refresh\" content=\"30\" /><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\"><style>  body   {text-align: center;}  table  {text-align: left;}  h1     {color: #fff;}  h2     {text-align: left;}  .lead  {background: white;display:  inline-block;color: #000; padding: 2px 10px;}iframe {border: none; width: 100%; height: 40vh;}</style></head><body>";
            page += "<div id=\"status\"><h2>Status</h2><hr /><table class=\"container\">";
            page += "<tr><td>Date - Time</td><td>";
            page += value.times.Day();
            page += "/";
            page += value.times.Month();
            page += "/";
            page += value.times.Year();
            page += " - ";
            page += value.times.Hour();
            page += ":";
            page += value.times.Minute();
            page += ":";
            page += value.times.Second();
            page += "</td>";
            page += "</tr>";
    
            page += "<tr><td>Humid</td><td>";
            page += value.humid;
            page += " %";
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-success\" role=\"progressbar\" aria-valuenow=\"";
            page += value.humid;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width:";
            page += value.humid;
            page += "%\">&nbsp</div></div>";
            page += "</td></tr>";
            
            page += "<tr><td>Temp</td><td>";
            page += value.temp;
            page += " C";
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-warning\" role=\"progressbar\" aria-valuenow=\"";
            page += value.temp;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width:";
            page += value.temp;
            page += "%\">&nbsp</div></div>";
            page += "</td></tr>";
            
            page += "<tr><td>Mois</td><td>";
            page += value.mois;
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-info\" role=\"progressbar\" aria-valuenow=\"";
            page += value.mois;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width: ";
            page += value.mois;
            page += "%\">&nbsp</div></div>";
            
            page += "</td></tr>";
            page += "</table>";
            page += "<br /><a class=\"btn btn-primary btn-md\" href=\"\" role=\"button\">Reload</a>";
            page += "</div>";
            page += "</body>";
            response(page, client);
          }else if(filename == "config"){
            String page = "<!DOCTYPE HTML> <html> <head> <meta http-equiv=\"refresh\" content=\"30\" /><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\"><style>  body   {text-align: center;}  table  {text-align: left;}  h1     {color: #fff;}  h2     {text-align: left;}  .lead  {background: white;display:  inline-block;color: #000; padding: 2px 10px;}iframe {border: none; width: 100%; height: 40vh;}</style></head><body>";
            page += "<div id=\"config\"><h2>config</h2><hr /><table class=\"container\">";

            page += "<tr><td>Mode</td><td>";
            page += conf.isslave;
            page += "</td></tr>";

            page += "<tr><td>Humid</td><td>";
            page += conf.humid;
            page += " %";
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-success\" role=\"progressbar\" aria-valuenow=\"";
            page += conf.humid;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width:";
            page += conf.humid;
            page += "%\">&nbsp</div></div>";
            page += "</td></tr>";
            
            page += "<tr><td>Temp</td><td>";
            page += conf.temp;
            page += " C";
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-warning\" role=\"progressbar\" aria-valuenow=\"";
            page += conf.temp;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width:";
            page += conf.temp;
            page += "%\">&nbsp</div></div>";
            page += "</td></tr>";
            
            page += "<tr><td>Mois</td><td>";
            page += conf.mois;
            page += "<div class=\"progress\"><div class=\"progress-bar progress-bar-striped progress-bar-animated bg-info\" role=\"progressbar\" aria-valuenow=\"";
            page += conf.mois;
            page += "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width: ";
            page += conf.mois;
            page += "%\">&nbsp</div></div>";
            
            page += "</td></tr>";
            page += "</table>";
            page += "<br /><a class=\"btn btn-primary btn-md\" href=\"\" role=\"button\">Reload</a>";
            page += "</div>";
            page += "</body>";
            response(page, client);
          }
        }
        Serial.println("HTTP Close");
        delay(300);
      }
  }
  return conf;
}

void response(String data, WiFiClient client){
  client.flush();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");               // Don't Forget Spliter
  client.print(data);
}



