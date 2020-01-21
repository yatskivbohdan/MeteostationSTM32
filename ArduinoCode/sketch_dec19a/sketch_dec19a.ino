#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


// Replace with your network credentials
const char* ssid = "NodeMCU";
const char* password = "11111111";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

 
String page, temp1, temp2, humidity, pressure, ttime, date;
int data[16] = {0}; 

void update_data(void){
   while (Serial.available()){
    int num = (int)Serial.read();
    int value = (int)Serial.read();
    int check = (int)Serial.read();
    if(value == check && value >= 0){
      if (data[num] != value){
      data[num] = value;
      }
    }
   }
  
   temp1 = String(data[0]) + "." + String(data[1]);
   humidity = String(data[2]) + "." + String(data[3]);
   temp2 = String(data[4]) +  "." + String(data[5]);
   if (data[7]<=9) 
      pressure = String(data[6]) + "0"+String(data[7]) + String(data[8]);
   else
        pressure = String(data[6]) + String(data[7]) + String(data[8]);
   String minutes;
   String seconds;
   String hours;
   String day;
   String month;
   if (data[9]<=9)
      hours = "0" + String(data[9]);
   else
      hours = String(data[9]);
   if (data[10]<=9)
      minutes = "0" + String(data[10]);
   else
      minutes = String(data[10]);
   if (data[11]<=9)
      seconds = "0" + String(data[11]);
   else
      seconds = String(data[11]);
   if (data[13]<=9)
      day = "0" + String(data[13]);
   else
      day = String(data[13]);
   if (data[14]<=9)
      month = "0" + String(data[14]);
   else
      month = String(data[14]);

    
   
   ttime = hours + ":" + minutes + ":" + seconds;
   date = weekday(data[12]) + ", " + day + "-" + month + "-" + String(2000+ data[15]);
   
}

String weekday(int number){
  switch (number){
    case 0:
        return "Sunday";
    case 1:
        return "Monday";
    case 2:
        return "Tuesday";
    case 3:
        return "Wednesday";
    case 4:
        return "Thursday";
    case 5:
        return "Friday";
    case 6:
        return "Saturday";
    default:
        return "Error";
  }
}
void send_ip(void){
      WiFi.mode(WIFI_STA);
      String ipadress = WiFi.localIP().toString();
      int ip_first, ip_second, ip_third, ip_fourth;
        
      ipadress += ".";
      String sended = "";
      int int_sended;
      int i = 0;
      int id_ip = 0;
      while (ipadress[i]) {
        if (ipadress[i] != '.'){
            sended += ipadress[i];
        }else{
          int_sended = sended.toInt();
          switch(id_ip){
            case 0: ip_first = int_sended; break;
            case 1: ip_second = int_sended; break;
            case 2: ip_third = int_sended; break;
            case 3: ip_fourth = int_sended; break;
            } 
          sended = ""; 
          ++id_ip;
        }
        ++i;
      }
      uint8_t buff[12] = {(uint8_t)ip_first, (uint8_t)ip_second, (uint8_t)ip_third, (uint8_t)ip_fourth};
      Serial.write(buff, sizeof(buff));

       
}



void setup(void){
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  server.on("/temp1", []() {
    server.send(200, "text/html", temp1);
  });
  
   server.on("/humidity", []() {
    server.send(200, "text/html", humidity);
  });
  
   server.on("/temp2", []() {
    server.send(200, "text/html", temp2);
  });
  
   server.on("/pressure", []() {
    server.send(200, "text/html", pressure);
  });
  
   server.on("/ttime", []() {
    server.send(200, "text/html", ttime);
  
  });
  server.on("/date", []() {
    server.send(200, "text/html", date);
  
  });

  server.on("/chart-temperature1", []() {
    if (data[0] != 0)
      server.send(200, "text/html", String(data[0]) + "." + String(data[1]));
  });

  server.on("/chart-humidity", []() {
    if (data[2] != 0)
    server.send(200, "text/html", String(data[2]) + "." + String(data[3]));
  });

  server.on("/chart-temperature2", []() {
    if (data[4] != 0)
    server.send(200, "text/html", String(data[4]) + "." + String(data[5]));
  });

   server.on("/chart-pressure", []() {
    if (data[6] != 0)
    server.send(200, "text/html", String(data[6]) + String(data[7])+ String(data[8]));
  });
  server.on("/", [](){
    page = "<!DOCTYPE html><html><head> <link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\"> \r\n";
    page += "<script src=\"https://code.highcharts.com/highcharts.js\"></script>";
    page += "<style>\r\n";
    page += "body {background-image: url(\"http://i.imgur.com/RnwnIzX.gif\");background-repeat: no-repeat; background-attachment: fixed; background-position: center;}\r\n";
    page += "html { font-family: Arial Black; display: inline-block; margin: 0px auto; text-align: center;}\r\n";
    page += "h2 { font-size: 3.0rem; } p { font-size: 3.0rem; }.units { font-size: 1.6rem; color:#ffffff;}.dht-labels{font-size: 1.5rem;vertical-align:middle;padding-bottom: 15px; color:#ffffff;}\r\n";
    page += "</style></head>\r\n";
    page += "<body><h1 style=\"color:white;\">Meteostation server</h1>";
    page += "<p><i class=\"fas fa-thermometer-half\" style=\"color:#ffffff;\"></i> <span class=\"dht-labels\">Temperature1</span> <span id=\"temp1\" style=\"color:white;\"></span><sup class=\"units\">&deg;C</sup></p>";
    page += "<p><i class=\"fas fa-tint\" style=\"color:#ffffff;\"></i> <span class=\"dht-labels\">Humidity</span> <span id=\"humidity\" style=\"color:white;\"></span><sup class=\"units\">%</sup></p>";
    page += "<p><i class=\"fas fa-thermometer-half\" style=\"color:#ffffff;\"></i> <span class=\"dht-labels\">Temperature2</span> <span id=\"temp2\" style=\"color:white;\"></span><sup class=\"units\">&deg;C</sup></p>";
    page += "<p><i class=\"fas fa-tachometer-alt\" style=\"color:#ffffff;\"></i> <span class=\"dht-labels\">Pressure</span> <span id=\"pressure\" style=\"color:white;\"></span><sup class=\"units\">Pa</sup></p>";
    page += "<p><i class=\"fas fa-clock\" style=\"font-size:1.0rem;color:#ffffff;\"></i> <span style=\"font-size:1.0rem; color:white\">Last changed</span> <span id=\"ttime\" style=\"font-size:1.0rem; color:white\"></span>";
    page += "<span style=\"font-size:1.0rem; color:white\"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><i class=\"fas fa-calendar-alt\" style=\"font-size:1.0rem;color:#ffffff;\"></i> <span style=\"font-size:1.0rem; color:white\"></span> <span id=\"date\" style=\"font-size:1.0rem; color:white\"></span></P>";

    page += "<h2 style=\"color:white;\">Changes history</h2>\r\n";
    page += "<div id=\"chart-temperature1\" class=\"container\"></div>\r\n";
    page += "<div id=\"chart-humidity\" class=\"container\"></div>";
    page += "<div id=\"chart-temperature2\" class=\"container\"></div>";
    page += "<div id=\"chart-pressure\" class=\"container\"></div>";        
    page += "</body><script>\r\n";
    
    page += "var x = setInterval(function() {loadData(\"temp1\",updateData1)}, 1000);\r\n";
    page += "var y = setInterval(function() {loadData(\"humidity\",updateData2)}, 1000);\r\n";
    page += "var z = setInterval(function() {loadData(\"temp2\",updateData3)}, 1000);\r\n";
    page += "var a = setInterval(function() {loadData(\"pressure\",updateData4)}, 1000);\r\n";
    page += "var b = setInterval(function() {loadData(\"ttime\",updateData5)}, 1000);\r\n";
    page += "var c = setInterval(function() {loadData(\"date\",updateData6)}, 1000);\r\n";
    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " }\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";
    page += "function updateData1(){\r\n";
    page += " document.getElementById(\"temp1\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
     page += "function updateData2(){\r\n";
    page += " document.getElementById(\"humidity\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
     page += "function updateData3(){\r\n";
    page += " document.getElementById(\"temp2\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "function updateData4(){\r\n";
    page += " document.getElementById(\"pressure\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "function updateData5(){\r\n";
    page += " document.getElementById(\"ttime\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "function updateData6(){\r\n";
    page += " document.getElementById(\"date\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    
    page += "var chartT = new Highcharts.Chart({\r\n";
    page += "chart:{ renderTo : \"chart-temperature1\" },\r\n";
    page += "title: { text: \"DHT22 Temperature\" },\r\n";
    page += "series: [{\r\n";
    page += "showInLegend: false,\r\n";
    page += "data: []\r\n";
    page += "}],\r\n";
    page += "plotOptions: {\r\n";
    page += "line: { animation: false,\r\n";
    page += "dataLabels: { enabled: true }\r\n";
    page += "},\r\n";
    page += "series: { color: \"#059e8a\" }\r\n";
    page += "},xAxis: { type: \"datetime\",\r\n";
    page += "dateTimeLabelFormats: { second: \"%H:%M:%S\" }\r\n";
    page += "},\r\n";
    page += "yAxis: {\r\n";
    page += "title: { text: \"Temperature (Celsius)\" }\r\n";
    page += "},\r\n";
    page += "credits: { enabled: false }\r\n";
    page += "});\r\n";
    
    page += "setInterval(function ( ) {\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function() {\r\n";
    page += "if (this.readyState == 4 && this.status == 200) {\r\n;";
    page += "var x = (new Date()).getTime(),\r\n";
    page += "y = parseFloat(this.responseText);\r\n";
    page += "if(chartT.series[0].data.length > 40) {\r\n";
    page += "chartT.series[0].addPoint([x, y], true, true, true);\r\n";
    page += "} else {\r\n";
    page += "chartT.series[0].addPoint([x, y], true, false, true);\r\n";
    page += "}\r\n";
    page += "}\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", \"/chart-temperature1\", true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}, 1000 ) ;\r\n";

     page += "var chartH = new Highcharts.Chart({\r\n";
    page += "chart:{ renderTo : \"chart-humidity\" },\r\n";
    page += "title: { text: \"Humidity\" },\r\n";
    page += "series: [{\r\n";
    page += "showInLegend: false,\r\n";
    page += "data: []\r\n";
    page += "}],\r\n";
    page += "plotOptions: {\r\n";
    page += "line: { animation: false,\r\n";
    page += "dataLabels: { enabled: true }\r\n";
    page += "},\r\n";
    page += "series: { color: \"#00008b\" }\r\n";
    page += "},xAxis: { type: \"datetime\",\r\n";
    page += "dateTimeLabelFormats: { second: \"%H:%M:%S\" }\r\n";
    page += "},\r\n";
    page += "yAxis: {\r\n";
    page += "title: { text: \"Humidity (%))\" }\r\n";
    page += "},\r\n";
    page += "credits: { enabled: false }\r\n";
    page += "});\r\n";
    
    page += "setInterval(function ( ) {\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function() {\r\n";
    page += "if (this.readyState == 4 && this.status == 200) {\r\n;";
    page += "var x = (new Date()).getTime(),\r\n";
    page += "y = parseFloat(this.responseText);\r\n";
    page += "if(chartH.series[0].data.length > 40) {\r\n";
    page += "chartH.series[0].addPoint([x, y], true, true, true);\r\n";
    page += "} else {\r\n";
    page += "chartH.series[0].addPoint([x, y], true, false, true);\r\n";
    page += "}\r\n";
    page += "}\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", \"/chart-humidity\", true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}, 1000 ) ;\r\n";

    page += "var chartT2 = new Highcharts.Chart({\r\n";
    page += "chart:{ renderTo : \"chart-temperature2\" },\r\n";
    page += "title: { text: \"BMP180 Temperature\" },\r\n";
    page += "series: [{\r\n";
    page += "showInLegend: false,\r\n";
    page += "data: []\r\n";
    page += "}],\r\n";
    page += "plotOptions: {\r\n";
    page += "line: { animation: false,\r\n";
    page += "dataLabels: { enabled: true }\r\n";
    page += "},\r\n"; 
    page += "series: { color: \"#e3a8c7\" }\r\n";
    page += "},xAxis: { type: \"datetime\",\r\n";
    page += "dateTimeLabelFormats: { second: \"%H:%M:%S\" }\r\n";
    page += "},\r\n";
    page += "yAxis: {\r\n";
    page += "title: { text: \"Temperature (Celsius)\" }\r\n";
    page += "},\r\n";
    page += "credits: { enabled: false }\r\n";
    page += "});\r\n";
    
    page += "setInterval(function ( ) {\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function() {\r\n";
    page += "if (this.readyState == 4 && this.status == 200) {\r\n;";
    page += "var x = (new Date()).getTime(),\r\n";
    page += "y = parseFloat(this.responseText);\r\n";
    page += "if(chartT2.series[0].data.length > 40) {\r\n";
    page += "chartT2.series[0].addPoint([x, y], true, true, true);\r\n";
    page += "} else {\r\n";
    page += "chartT2.series[0].addPoint([x, y], true, false, true);\r\n";
    page += "}\r\n";
    page += "}\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", \"/chart-temperature2\", true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}, 1000 ) ;\r\n";

    page += "var chartP = new Highcharts.Chart({\r\n";
    page += "chart:{ renderTo : \"chart-pressure\" },\r\n";
    page += "title: { text: \"Pressure\" },\r\n";
    page += "series: [{\r\n";
    page += "showInLegend: false,\r\n";
    page += "data: []\r\n";
    page += "}],\r\n";
    page += "plotOptions: {\r\n";
    page += "line: { animation: false,\r\n";
    page += "dataLabels: { enabled: true }\r\n";
    page += "},\r\n";
    page += "series: { color: \"#f7dc68\" }\r\n";
    page += "},xAxis: { type: \"datetime\",\r\n";
    page += "dateTimeLabelFormats: { second: \"%H:%M:%S\" }\r\n";
    page += "},\r\n";
    page += "yAxis: {\r\n";
    page += "title: { text: \"Pressure (Pa)\" }\r\n";
    page += "},\r\n";
    page += "credits: { enabled: false }\r\n";
    page += "});\r\n";
    
    page += "setInterval(function ( ) {\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function() {\r\n";
    page += "if (this.readyState == 4 && this.status == 200) {\r\n;";
    page += "var x = (new Date()).getTime(),\r\n";
    page += "y = parseInt(this.responseText);\r\n";
    page += "if(chartP.series[0].data.length > 40) {\r\n";
    page += "chartP.series[0].addPoint([x, y], true, true, true);\r\n";
    page += "} else {\r\n";
    page += "chartP.series[0].addPoint([x, y], true, false, true);\r\n";
    page += "}\r\n";
    page += "}\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", \"/chart-pressure\", true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}, 1000 ) ;\r\n";

    

   
    
    
    
    page += "</script></html>\r\n";

    server.send(200, "text/html", page);
  });
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();
  send_ip();
  delay(100);
  update_data();
    
}
