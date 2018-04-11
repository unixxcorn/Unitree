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

void fileOverwrite(String filename, String data){
  SD.remove(filename.c_str());
  fileWrite(filename, data);
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

void StrToInt(String text){/*ex-input "100,50,30,1" or "100,100,100,100"*/
    char* data = strdup(text.c_str());
    int i, j, k, check = 0, stard = 0, order = 1 ,memo[4] = {0, 0, 0, 0};
    for(i = 0; i < 17;i++){
        if(data[i] == ',' || data[i] == '\0'){
            for(j = stard;j < i;j++){
                if(i - stard == 1 && check == 0){
                  order = 1;
                  check = 1;
                }if(i - stard == 2 && check == 0){
                  order = 10;
                  check = 1;
                }if(i - stard == 3 && check == 0){
                  order = 100;
                  check = 1;
                }
                memo[k] += (data[j] - 48) * order;
                order /= 10;
                if(data[i] == '\0'){break;}
            }
            check = 0;
            stard = i+1;
            k++;
        }
    }
    conf.isslave = memo[0];
    conf.humid = memo[1];
    conf.temp = memo[2];
    conf.mois = memo[3];
}
