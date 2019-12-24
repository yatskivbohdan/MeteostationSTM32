#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


// Replace with your network credentials
const char* ssid = "NodeMCU";
const char* password = "11111111";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String Website, temp1, temp2, humidity, pressure;
int data[9] = {0};
String Default = "<h1>Meteostation server</h1> <head> <meta http-equiv= \"refresh\" content=\"2\"> </head>";

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
      Website=Default;
      temp1 =" " + String(data[0]) + "." + String(data[1]) + "C ";
      humidity =" " + String(data[2]) + "." + String(data[3]) + "% ";
      temp2 = " " + String(data[4]) + "." + String(data[5]) + "C ";
      pressure = " " + String(data[6])+String(data[7])+String(data[8]) + "Pa";
      Website += "<p>Temperature:" + temp1 + "</p>";
      Website += "<p>Humidity:" + humidity + "</p>";
       Website += "<p>Temperature2:" + temp2 + "</p>";
      Website += "<p>Pressure:" + pressure + "</p>";
    }
    
  
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
    server.send(200, "text/html", Website);
  });
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();
  delay(100);
  update_data();
}
