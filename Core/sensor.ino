sensor readSensor(){
  sensor value;
  value.times = rtc.GetDateTime();
  value.humid = dht.readHumidity();
  value.temp  = dht.readTemperature();  
  value.mois  = ((1024 - analogRead(yl_apin))/10);
  return value;
}

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

