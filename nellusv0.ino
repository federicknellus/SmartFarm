#include <ESP8266WiFi.h>
const char* ssid = "";//your-ssid
const char* password = "";//your-password
WiFiServer server(80);
const int humidity_sensor = A0;
const int relay = 16;
const int floating_sensor = 14;
int humidity;
void setup()
{ 
  pinMode(humidity_sensor,INPUT);
  pinMode(relay, OUTPUT);
  pinMode(floating_sensor,INPUT_PULLUP);
  pumpoff();
  Serial.begin(115200);
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){delay(500);Serial.print(".");}
  Serial.println("WiFi connected");  
  server.begin();  // Starts the Server
  Serial.println("Server started");

  Serial.print("IP Address of network: "); // Prints IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop()
{WiFiClient client = server.available();
  if (!client){return;}
  while(!client.available()){delay(1);}
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  humidity = analogRead(humidity_sensor);

  
  if(request.indexOf("/PUMP-ON") != -1)
  {
    pumpon(); // Turn ON LED
    
  }
  if(request.indexOf("/PUMP-OFF") != -1)
  {
    pumpoff(); // Turn OFF LED
  }
  if(request.indexOf("/GET-HUMIDITY") != -1)
  {
    ; // Turn ON LED
    
  }

/*------------------HTML Page Creation---------------------*/

  client.println("HTTP/1.1 200 OK"); // standalone web server with an ESP8266
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
  client.println("<title>Nellus v0</title>");
  client.println("</head>");
  client.print("<body>");
  client.println("<header id=\"main-header\">");
  client.println("<h1>Nellus v0</h1>");
  client.println("<h4>by Nellus</h4>");
  client.println("</header>");
   
  /*if(state == on)
  {
    client.print("ON");
  }
  else
  {
    client.print("OFF");
  }*/
  client.print("<br>");
  client.print("<br>");
  client.println("<a href=\"/PUMP-ON\"\"><button class=\"button\">PUMP-ON</button></a>");
  client.println("<a href=\"/PUMP-OFF\"\"><button class=\"button\">PUMP-OFF</button></a>");
  client.println("<a href=\"/GET-HUMIDITY\"\"><button class=\"button\">GET-HUMIDITY</button></a>");
      client.println("<h4>uuu</h4>");


  client.print("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
void pumpon(){digitalWrite(relay,LOW);}
void pumpoff(){digitalWrite(relay,HIGH);}
bool waterhigh(){if(digitalRead(floating_sensor)==0){return true;}else{return false;} }

String SendHTML(float HumidityWeb, String TimeWeb,String DateWeb){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Global Server</title>\n";

  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 Global Server</h1>\n";

  ptr +="<p>Date: ";
  ptr +=(String)DateWeb;
  ptr +="</p>";
  ptr +="<p>Time: ";
  ptr +=(String)TimeWeb;
  ptr +="</p>";
  
  ptr +="<p>Humidity: ";
  ptr +=(int)HumidityWeb;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
