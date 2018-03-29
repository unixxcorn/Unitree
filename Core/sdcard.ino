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
    conf.isslave = confi[0];
    conf.humid = confi[1];
    conf.temp = confi[2];
    conf.mois = confi[3];
    return conf;
  }else{
    file = SD.open(fileName, FILE_WRITE);
    file.printf("%c%c%c%c", 0 , 30 , 30 , 30);
    file.close();
    configRead(fileName);
  }
}
