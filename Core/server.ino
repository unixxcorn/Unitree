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
          
          if(request[i++] == '?'){
            String getVal = "";
            while(request[i] != ' '){
              getVal += request[i];
              i++;
            }
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
            String page = "<!DOCTYPE HTML><html><head><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\"><style>  body   {text-align: center;}  table  {text-align: left;}  h1     {color: #fff;}  h2     {text-align: left;}  .lead  {background: white;display:  inline-block;color: #000; padding: 2px 10px;}iframe {border: none; width: 100%; height: 40vh;}</style></head><body>";
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
        delay(100);
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


