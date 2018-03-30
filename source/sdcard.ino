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

void logFile(String msg){
    String temp = "";temp += value.times.Year();
    temp += "-";temp += value.times.Month();
    temp += "-";temp += value.times.Day();
    temp += " - ";temp += value.times.Hour();
    temp += ":";temp += value.times.Minute();
    temp += ":";temp += value.times.Second();
    temp += ",";temp += value.temp;
    temp += ",";temp += value.humid;
    temp += ",";temp += value.mois;
    temp += ",";temp += msg;
    fileWrite("water.log", temp);
}

