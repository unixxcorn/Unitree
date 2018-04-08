void webserve(){
  server.on("/", handleSDCard);
  server.on("/slave", handleSlave);
  server.on("/status", handleStatus);
  server.on("/config", handleConfig);
  server.onNotFound(handleSDCard);
  Serial.println("HTTP server started");
  
  server.begin();
}

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSlave(){
  server.send(200, "text/json", "{\"name\":\"John\",\"age\":30,\"cars\":[ \"Ford\", \"BMW\", \"Fiat\" ]}");
}

void handleStatus(){
  sensor value = readSensor();
  String temp = "{";
  temp += "\"time\":\"";   temp += value.times.Year();
  temp += "-";  temp += value.times.Month();
  temp += "-";  temp += value.times.Day();
  temp += " ";  temp += value.times.Hour();
  temp += ":";  temp += value.times.Minute();
  temp += ":";  temp += value.times.Second();
  temp += "\",\"temp\":"; temp += value.temp;
  temp += ",\"humid\":"; temp += value.humid;
  temp += ",\"mois\":"; temp += value.mois;
  temp += "}";
  server.send(200, "text/json", temp);
}

void handleConfig(){
  Serial.println(( server.method() == HTTP_GET ) ? "GET" : "POST");
  if(server.method() != HTTP_GET){
    Serial.println(server.args());
    String val = "";
    for (int i = 0; i < server.args(); i++) {
      Serial.println(server.argName(i)+": "+server.arg(i));
    }
    val += server.arg(3);
    val += server.arg(0);
    val += server.arg(1);
    val += server.arg(2);
    fileOverwrite("config.utc", val);
  }
  conf = configRead("config.utc");
  String temp = "{";
  temp += "\"IP Address\":\"";temp += (char*) WiFi.localIP().toString().c_str();
  temp += "\",\"temp\":\"";   temp += conf.temp;
  temp += "\",\"humid\":\"";  temp += conf.humid;
  temp += "\",\"mois\":\"";   temp += conf.mois;
  temp += "\"}";
  server.send(200, "text/json", temp);
}

void handleSDCard(){
  if(loadFromSdCard(server.uri())) return;
  String message = "Not Found! "+server.uri();
  server.send(404, "text/plain", message);
  Serial.println(message);
}

bool loadFromSdCard(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".log")) dataType = "text/plain";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  
  File dataFile = SD.open(path.c_str());
  if(dataFile.isDirectory()){
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }
  
  if (!dataFile){
    return false;
  }
  server.streamFile(dataFile, dataType);
  dataFile.close();
  
  return true;
}
