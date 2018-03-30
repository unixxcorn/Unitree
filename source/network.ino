void wifi_connect(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  unsigned int i = 0;
  Serial.printf("Connecting to %s\n", ssid);
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    Serial.printf("|");
    delay(500);
  }
  Serial.print("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());
}


