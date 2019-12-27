#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid = "NodeMCU";
const char* password = "11111111";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String Website, temp1, temp2, humidity, pressure, Final;
int data[9] = {0};
String Default = "<h1>Meteostation server</h1> <head> <meta http-equiv= \"refresh\" content=\"1\"> </head>";
//String datestr;
int year, month, day;
String last_temp, last_humidity, last_temp2, last_press;  

const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,utcOffsetInSeconds);

void update_data(void){
   bool changed = false;
   while (Serial.available()){
    changed = false;
    int num = (int)Serial.read();
    int value = (int)Serial.read();
    int check = (int)Serial.read();
    if(value == check && value >= 0){
      if (data[num] != value){
      data[num] = value;
      changed = true;
      }
    }
   }
    if (changed){
      Website = Default;
      temp1 =" " + String(data[0]) + "." + String(data[1]) + "C ";
      humidity =" " + String(data[2]) + "." + String(data[3]) + "% ";
      temp2 = " " + String(data[4]) + "." + String(data[5]) + "C ";
      pressure = " " + String(data[6])+String(data[7])+String(data[8]) + "Pa";
      Website += "<p>Temperature:" + temp1 + " previous was:" + last_temp + "</p>";
      Website += "<p>Humidity:" + humidity + " previous was:" + last_humidity + "</p>";
       Website += "<p>Temperature2:" + temp2 + " previous was:" + last_temp2 + "</p>";
      Website += "<p>Pressure:" + pressure + " previous was:" + last_press + "</p>";
      if (temp1 != last_temp){
        last_temp = temp1;
      }
     if (temp2 != last_temp2){
      last_temp2 = temp2;
      }
    if (humidity != last_humidity){
      last_humidity = humidity;
    }
    if (pressure != last_press){
      last_press = pressure;
    }
      
    
    }
    
    Final = Website + "<p>"+ timeClient.getFormattedTime() + "</p>" + "<p>" + day + "-" + month + "-" + year + "</p>";

     
}

void send_time(){
    String timestr = timeClient.getFormattedTime();
    timestr += ":";
    int hours, mins, sec;  
  // ipadress += ".";
    String sended = "";
    int int_sended;
    int i = 0;
    int id_time = 0;
  while (timestr[i]) {
    if (timestr[i] != ':'){
        sended += timestr[i];
    }else{
      int_sended = sended.toInt();
      switch(id_time){
        case 0: hours = int_sended; break;
        case 1: mins = int_sended; break;
        case 2: sec = int_sended; break;
        } 
      sended = ""; 
      ++id_time;
    }
    ++i;
  }
  uint8_t buf[3] = {(uint8_t)hours, (uint8_t)mins, (uint8_t)sec};
  Serial.write(buf, sizeof(buf));
  delay(100);
    
}


  void send_ip_and_time(void){
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
      String timestr = timeClient.getFormattedTime();
    timestr += ":";
    int hours, mins, sec;  
  // ipadress += ".";
    sended = "";
    i = 0;
    int id_time = 0;
  while (timestr[i]) {
    if (timestr[i] != ':'){
        sended += timestr[i];
    }else{
      int_sended = sended.toInt();
      switch(id_time){
        case 0: hours = int_sended; break;
        case 1: mins = int_sended; break;
        case 2: sec = int_sended; break;
        } 
      sended = ""; 
      ++id_time;
    }
    ++i;
  }
  String formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  String datestr = formattedDate.substring(0, splitT);
    datestr += "-";
   
  // ipadress += ".";
    sended = "";
    i = 0;
    int id_date = 0;
  while (datestr[i]) {
    if (datestr[i] != '-'){
        sended += datestr[i];
    }else{
      int_sended = sended.toInt();
      switch(id_date){
        case 0: year = int_sended; break;
        case 1: month = int_sended; break;
        case 2: day = int_sended; break;
        } 
      sended = ""; 
      ++id_date;
    }
    ++i;
  }
       uint8_t buff[12] = {(uint8_t)ip_first, (uint8_t)ip_second, (uint8_t)ip_third, (uint8_t)ip_fourth, (uint8_t)hours, (uint8_t)mins, (uint8_t)sec, (uint8_t)(year%100), (uint8_t)month, (uint8_t)day, (uint8_t)timeClient.getDay()};
     
        
       Serial.write(buff, sizeof(buff));

       
}  

void setup(void){
  //the HTML of the web page
  Website=Default;
  //page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  
   
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", [](){
    server.send(200, "text/html", Final);
  });
  server.begin();
  timeClient.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
  server.handleClient();
  send_ip_and_time();
  delay(100);
  update_data();
    
}
