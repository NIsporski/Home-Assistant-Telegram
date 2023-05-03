/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include "DHT.h"
#include <IRsend.h>
#include <ir_Gree.h>  

DHT dht;

IRsend irsend(D2);

// Set web server port number to 80
WiFiServer server(80);

// Replace with your network credentials
//const char* ssid     = "AndroidAPA810"; // Enter your WIFI name
//const char* password = "Buntovna_5"; // Enter your WIFI password

const char* ssid     = "A1_87BB"; // Enter your WIFI name
const char* password = "48575443D85C8CA4"; // Enter your WIFI password

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output1State = "off";

// Initialize Telegram BOT
#define BOTtoken "6122016934:AAH4-cO3H3s726q5IvNTTOo5yBIj94T3aMk"  // nisporski_bot
//#define BOTtoken "6220835657:AAHRPDu9SS7YYLSrGRDf_fHly7ti9M_0FoQ"  // iasmarthome_bot

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can message you
#define CHAT_ID_IA "1845399208"
#define CHAT_ID_NI "2077424490"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = D0;
bool ledState = HIGH;

// Assign output variables to GPIO pins
const int ledPin1 = D1;
bool ledState1 = HIGH;

// Current time
unsigned long currentTime = millis();
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

float temperature;
float humidity;
String DHTStatus;

IRGreeAC ac(D2);  // Set the GPIO to be used for sending messages.

#define POWER  (0xE0E040BF)
#define POWER_OFF (0xE0E019E6)
#define POWER_ON ( 0xE0E09966)
#define LEFT (0xE0E0A659)
#define RIGHT (0xE0E046B9)
#define UP (0xE0E006F9)
#define DOWN (0xE0E08679)
#define ENTER (0xE0E016E9)
#define MENU (0xe0e058a7)
#define TOOLS (0xE0E0D22D)
#define EXIT (0xE0E0B44B)
#define RETURN (0xE0E01AE5)
#define SOURCE (0xE0E0807F)
#define EXIT (0xE0E0B44B)
#define VOLUME_UP (0xE0E0E01F)
#define VOLUME_DOWN (0xE0E0D02F)
#define MUTE (0xE0E0F00F)

void Samsung_Send_Code(uint32_t code, uint32_t ms)
{
  irsend.sendSAMSUNG(code, 32);
  delay(ms);
}

void Gree_On()
{
    ac.on();
    ac.send();
}

void Gree_Off()
{
    ac.off();
    ac.send();
}

void Gree_Cool()
{
    //ac.on();
    ac.setMode(kGreeCool);
    ac.setTemp(18);
    ac.setXFan(false);
    ac.setLight(true);
    ac.setSleep(false);
    ac.setTurbo(false);
    ac.setDisplayTempSource(kGreeDisplayTempSet);
    ac.send();
}

void Gree_Heat()
{
    //ac.on();
    ac.setMode(kGreeHeat);
    ac.setTemp(24);
    ac.setXFan(false);
    ac.setLight(true);
    ac.setSleep(false);
    ac.setTurbo(false);
    ac.setDisplayTempSource(kGreeDisplayTempSet);
    ac.send();
}

void TurnOnOffSonyTV();

bool IsAuthorized(String chat_id)
{
  if (chat_id != CHAT_ID_IA && chat_id != CHAT_ID_NI)
    return false;
  return true;
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if(!IsAuthorized(chat_id))
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String keyboardJson = "[[\"/led1_on\", \"/led1_off\"],[\"/LED_state\"], [\"/temp\", \"/hum\"], [\"/tv_on\", \"/tv_off\"], [\"/tv_left\", \"/tv_right\"], [\"/tv_up\", \"/tv_down\"], [\"/tv_enter\", \"/tv_exit\"], [\"/tv_vol_up\", \"/tv_vol_down\"], [\"/tv_mute\"], [\"/AC_on\", \"/AC_off\"], [\"/AC_cool\", \"/AC_heat\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }

    if (text == "/led1_on") {
      bot.sendMessage(chat_id, "LED1 state set to ON", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led1_off") {
      bot.sendMessage(chat_id, "LED1 state set to OFF", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/LED_state") {
      if (!digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED1 is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED1 is OFF", "");
      }
      return;
    }

    //delay(dht.getMinimumSamplingPeriod()); /* Delay of amount equal to sampling period */

    if (text == "/temp") {
      if(DHTStatus == "OK")
      {
        String temp = String(temperature);
        bot.sendMessage(chat_id, "Temperature : " + temp, "");        
      }
      else
      {
        bot.sendMessage(chat_id, "Can't measure temperature, sensor status: " + DHTStatus, ""); 
      }
      return;
      
    }

    if (text == "/hum") {
      if(DHTStatus == "OK")
      {
          String hum = String(humidity);
        bot.sendMessage(chat_id, "Humidity : " + hum, "");
      }
      else
      {
        bot.sendMessage(chat_id, "Can't measure humidity, sensor status: " + DHTStatus, ""); 
      }
      return;
    }
  
    if (text == "/tv_on") {
        bot.sendMessage(chat_id, "TV turned on", "");
        Samsung_Send_Code(POWER_ON, 1000);
        return;
    }

    if (text == "/tv_off") {
        bot.sendMessage(chat_id, "TV turned off", "");
        Samsung_Send_Code(POWER_OFF, 1000);
        return;
    }

    if (text == "/tv_left") {
        bot.sendMessage(chat_id, "TV left", "");
        Samsung_Send_Code(LEFT, 100);
        return;
    }

    if (text == "/tv_right") {
        bot.sendMessage(chat_id, "TV right", "");
        Samsung_Send_Code(RIGHT, 100);
        return;
    }

    if (text == "/tv_up") {
        bot.sendMessage(chat_id, "TV up", "");
        Samsung_Send_Code(UP, 100);
        return;
    }

    if (text == "/tv_down") {
        bot.sendMessage(chat_id, "TV down", "");
        Samsung_Send_Code(DOWN, 100);
        return;
    }

    if (text == "/tv_enter") {
        bot.sendMessage(chat_id, "TV enter", "");
        Samsung_Send_Code(ENTER, 100);
        return;
    }

    if (text == "/tv_exit") {
        bot.sendMessage(chat_id, "TV exit", "");
        Samsung_Send_Code(EXIT, 100);
        return;
    }

      if (text == "/tv_vol_up") {
        bot.sendMessage(chat_id, "TV volume up", "");
        Samsung_Send_Code(VOLUME_UP, 100);
        return;
    }

    if (text == "/tv_vol_down") {
        bot.sendMessage(chat_id, "TV volume down", "");
        Samsung_Send_Code(VOLUME_DOWN, 100);
        return;
    }

    if (text == "/tv_mute") {
        bot.sendMessage(chat_id, "TV mute", "");
        Samsung_Send_Code(MUTE, 100);
        return;
    }

    // gree control
    if (text == "/AC_on") {
        bot.sendMessage(chat_id, "AC On", "");
        Gree_On();
        return;
    }

    if (text == "/AC_off") {
        bot.sendMessage(chat_id, "AC Off", "");
        Gree_Off();
        return;
    }

    if (text == "/AC_cool") {
        bot.sendMessage(chat_id, "AC Cool mode 18 degrees", "");
        Gree_Cool();
        return;
    }

    if (text == "/AC_heat") {
        bot.sendMessage(chat_id, "AC Heat mode 24 degrees", "");
        Gree_Heat();
        return;
    }

    // if (text == "/options")
    // {
    //   String keyboardJson = "[[\"/led1_on\", \"/led1_off\"],[\"/state\"], [\"/temp\", \"/hum\"], [\"/tv_on\", \"/tv_off\"], [\"/tv_left\", \"/tv_right\"], [\"/tv_up\", \"/tv_down\"], [\"/tv_enter\", \"/tv_exit\"], [\"/tv_vol_up\", \"/tv_vol_down\"], [\"/tv_mute\"], [\"/gree_on\", \"/gree_off\"], [\"/gree_cool\", \"/gree_heat\"]]";
    //   bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    // }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  // D0
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // D1
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, ledState1);
  
    // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  server.begin();

  // setup the DHT11/DHT22 sensor
  dht.setup(D3); 	/* D3 is used for data communication */
  Serial.println("Status\tHumidity (%)\tTemperature (C)");

  irsend.begin();
  ac.begin();
}

void HandleTelegram()
{
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void HandleWebServer()
{
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("GPIO 1 on");
              output1State = "on";
              digitalWrite(ledPin1, LOW);
            } else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 1 off");
              output1State = "off";
              digitalWrite(ledPin1, HIGH);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Welcome to IOT system</h1>");

            // Display current state, and ON/OFF buttons for GPIO 0
            client.println("<p>Relay 2 " + output1State + "</p>");
            // If the output0State is off, it displays the ON button
            if (output1State == "off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void TurnOnOffSonyTV()
{
      irsend.sendSony(0xa90, 12); // code for Sony TV power command
      delay(5000); // wait 5 seconds
}

void loop() {
  
  delay(dht.getMinimumSamplingPeriod()); /* Delay of amount equal to sampling period */
  humidity = dht.getHumidity();/* Get humidity value */
  temperature = dht.getTemperature();/* Get temperature value */
  DHTStatus = dht.getStatusString();

  HandleTelegram();

  // check also for incoming clients through the web server and handle them
  //HandleWebServer();

  //Serial.println("Sending IR command to A/C ...");
  //irsend.send();
}