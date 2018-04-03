sensor readSensor(){
  sensor value;
  value.times = rtc.GetDateTime();
  value.humid = dht.readHumidity();
  value.temp  = dht.readTemperature();  
  value.mois  = map(1024 - analogRead(yl_apin), 0, 1024, 0 , 100);
  return value;
}

void bootSensor(){
  Serial.println("Checking Sensor"); 
  dht.begin();  // start DHT Sensor
  rtc.Begin();  // start RTC (using I2C)
  delay(300);
}

void bootOutput(){
  Serial.println("Checking Output");
  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin, 1);
  delay(300);
  digitalWrite(relaypin, 0);
}

void output_control(sensor value, configure conf){
  if(value.humid < conf.humid || value.temp > conf.temp || value.mois < conf.mois){
    logFile("Water");
    digitalWrite(relaypin, 1);

  }else{
    if(value.humid > conf.humid){
      logFile("Humidity too high!");
    }if(value.temp < conf.temp){
      logFile("Temperature too low!");
    }if(value.mois > conf.humid){
      logFile("Moisture too high!");
    }
    digitalWrite(relaypin, 0);
  }
}

void printSensor(sensor value){
  String temp = "";temp += value.times.Year();
  temp += "-";temp += value.times.Month();
  temp += "-";temp += value.times.Day();
  temp += " - ";temp += value.times.Hour();
  temp += ":";temp += value.times.Minute();
  temp += ":";temp += value.times.Second();
  temp += ",";temp += value.temp;
  temp += ",";temp += value.humid;
  temp += ",";temp += value.mois;
  Serial.println(temp);
}

