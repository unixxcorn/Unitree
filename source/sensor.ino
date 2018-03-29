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
  if(value.humid > conf.humid || value.temp > conf.temp || value.mois < conf.mois){
    digitalWrite(relaypin, 1);
  }else{
    digitalWrite(relaypin, 0);
  }
  
}
