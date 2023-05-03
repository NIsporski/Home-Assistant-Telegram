#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid     = "XXX"; // Enter your WIFI name
const char* password = "XXX"; // Enter your WIFI password
String  command =""; // Command received from Android device

// Set Relay Pins
int relay1 = 16;
int relay2 = 5;
int relay3 = 4;
int relay4 = 0;
int relay5 = 2;
int relay6 = 14;
int relay7 = 12;
int relay8 = 13;

void setup()
{
  Serial.begin(115200);

  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);  
  pinMode(relay3, OUTPUT);  
  pinMode(relay4, OUTPUT);  
  pinMode(relay5, OUTPUT); 
  pinMode(relay6, OUTPUT);  
  pinMode(relay7, OUTPUT);  
  pinMode(relay8, OUTPUT);  
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
  digitalWrite(relay5,HIGH);
  digitalWrite(relay6,HIGH);
  digitalWrite(relay7,HIGH);
  digitalWrite(relay8,HIGH);
      
  connectWiFi();
  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient1 ();

         if (command == "r1on"  )     {digitalWrite(relay1,LOW); Serial.println("Relay1 is on.");}
    else if (command == "r1off" )     {digitalWrite(relay1,HIGH);Serial.println("Relay1 is off.");}
    else if (command == "r2on"  )     digitalWrite(relay2,LOW);     
    else if (command == "r2off" )     digitalWrite(relay2,HIGH);
    else if (command == "r3on"  )     digitalWrite(relay3,LOW);
    else if (command == "r3off" )     digitalWrite(relay3,HIGH);
    else if (command == "r4on"  )     digitalWrite(relay4,LOW);
    else if (command == "r4off" )     digitalWrite(relay4,HIGH);
    else if (command == "r5on"  )     digitalWrite(relay5,LOW);
    else if (command == "r5off" )     digitalWrite(relay5,HIGH);
    else if (command == "r6on"  )     digitalWrite(relay6,LOW);
    else if (command == "r6off" )     digitalWrite(relay6,HIGH);
    else if (command == "r7on"  )     digitalWrite(relay7,LOW);
    else if (command == "r7off" )     digitalWrite(relay7,HIGH);
    else if (command == "r8on"  )     digitalWrite(relay8,LOW);
    else if (command == "r8off" )     digitalWrite(relay8,HIGH);
    else if (command == "alloff") 
    {
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
      digitalWrite(relay3,HIGH);
      digitalWrite(relay4,HIGH);
      digitalWrite(relay5,HIGH);
      digitalWrite(relay6,HIGH);
      digitalWrite(relay7,HIGH);
      digitalWrite(relay8,HIGH);
    }
    else if (command == "allon") 
    {
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      digitalWrite(relay3,LOW);
      digitalWrite(relay4,LOW);
      digitalWrite(relay5,LOW);
      digitalWrite(relay6,LOW);
      digitalWrite(relay7,LOW);
      digitalWrite(relay8,LOW);
    }
    sendBackEcho(command); // send command echo back to android device
    command = "";
}

/* connecting WiFi */
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}

/* check command received from Pi */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  
  String request = client.readStringUntil('\r');

  Serial.print("Client request: [");
  Serial.print(request);
  Serial.println("]");

  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

unsigned long currentTime = millis();
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

String checkClient1(void)
{
  while(!client.available()) delay(1); 

  /*String request = "";

  Serial.println("New Client.");          // print a message out in the serial port
    currentTime = millis();
    previousTime = currentTime;
  while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c != '\r') 
        {  // if you got anything else but a carriage return character,
          request += c;      // add it to the end of the currentLine
        }
      }
    }
*/
  Serial.println("Client detected, reading request");
  String request = client.readStringUntil('\r');

  request.remove(0, 5);
  request.remove(request.length()-9,9);
  Serial.println(request);
  return request;
}

/* send command echo back to Pi */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println(echo);
  client.stop();
  delay(1);
}