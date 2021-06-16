/* Here is example yaml to add to Home Assistant to read the address and state of the sensors
   
binary_sensor:
  
     
*/
//#include <SimpleTimer.h>    //https://github.com/marcelloromani/Arduino-SimpleTimer/tree/master/SimpleTimer
#include <ESP8266WiFi.h>    //if you get an error here you need to install the ESP8266 board manager 
#include <ESP8266mDNS.h>    //if you get an error here you need to install the ESP8266 board manager 
#include <PubSubClient.h>   //https://github.com/knolleary/pubsubclient
#include <ArduinoOTA.h>     //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
// #include <Wire.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include "logix_secret.h" // CONFIG SECTION

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h" // library for player
#include <Adafruit_NeoPixel.h> // library for eyes
// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 4; // Connects to module's RX uno=2 nodemcu D2 = 4
static const uint8_t PIN_MP3_RX = 0; // Connects to module's TX uno=3 nodemuc D3 = 0
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;
// LED eyes bellow
#define PINR      14
#define PINL      12
#define NUMPIXELS 3
Adafruit_NeoPixel pixelsr(NUMPIXELS, PINR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsl(NUMPIXELS, PINL, NEO_GRB + NEO_KHZ800);
// bellow for PIR
#define pirPin 13
int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;
int PIRValue = 0;



/*****************  START USER CONFIG SECTION *********************************/
/*****************  START USER CONFIG SECTION *********************************/


/*****************  END USER CONFIG SECTION *********************************/
/*****************  END USER CONFIG SECTION *********************************/

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
// SimpleTimer timer;

//Global Variables
bool boot = true;
char sensorPublish[50];
char charPayload[50];
char * stateme[2] = {"OFF", "ON"};  // CHANGE TO OPEN OR CLOSE IF YOU LIKE



const char* ssid = USER_SSID ; 
const char* password = USER_PASSWORD ;
const char* mqtt_server = USER_MQTT_SERVER ;
const int mqtt_port = USER_MQTT_PORT ;
const char *mqtt_user = USER_MQTT_USERNAME ;
const char *mqtt_pass = USER_MQTT_PASSWORD ;
const char *mqtt_client_name = USER_MQTT_CLIENT_NAME ; 
const int bouncedelay = 150;  // THIS IS A DELAY TO TRY AND PREVENT DOUBLE PRESS OR SENSOR BOUNCE - PROBABLY A BETTER WAY 

 
int i;


//Functions
void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
// put wifi connect voice here
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() 
{
  int retries = 0;
  while (!client.connected()) {
    if(retries < 150)
    {
      Serial.print("Attempting MQTT connection...");
      if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass)) 
      {
        Serial.println("connected");
 // put connected in here
        if(boot == false)
        {
          client.publish(USER_MQTT_CLIENT_NAME"/checkIn","Reconnected"); 
        }
        if(boot == true)
        {
          client.publish(USER_MQTT_CLIENT_NAME"/checkIn","Rebooted");
        }
        // ... and resubscribe
        client.subscribe(USER_MQTT_CLIENT_NAME"/Command");
        } 
      else 
      {
   // put failed msg in here
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        retries++;
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    if(retries > 149)
    {
    ESP.restart();
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print(" therefore ");
  switch ((char)payload[0]) {
   case '1':
     Serial.println("Playing Sounds bite 1......");
     player.play(1);
     break;
   case '2':
     Serial.println("Playing Sounds bite 2......");
     player.play(2);
     break;
   case '3':
     Serial.println("Playing Sounds bite 3......");
     player.play(3);
     
     break;
   case '4':
     Serial.println("Playing Sounds bite 4......");
     player.play(4);
     break;
   case '5':
     Serial.println("Playing Sounds bite 5......");
     player.play(5);
     break;
   case '6':
     Serial.println("Playing Sounds bite 6......");
     player.play(6);
     break;
   case '7':
     Serial.println("Playing Sounds bite 7......");
     player.play(7);
     break;
   case '8':
     Serial.println("Playing Sounds bite 8......");
     player.play(8);
     break;
   case '9':
     Serial.println("Playing Sounds bite 9......");
     player.play(9);
     break;
   case '0':
     Serial.println("Playing Sounds bite 10......");
     player.play(10);
     break;
   case 'A':
     Serial.println("Playing Sounds bite 11......");
     player.play(11);
     break;
   case 'B':
     Serial.println("Playing Sounds bite 12......");
     player.play(12);
     break;
   case 'C':
     Serial.println("Playing Sounds bite 13......");
     player.play(13);
     break;
   case 'D':
     Serial.println("Playing Sounds bite 14......");
     player.play(14);
     break;
   case 'E':
     Serial.println("Playing Sounds bite 15......");
     player.play(15);
     break;
   case 'F':
     Serial.println("Playing Sounds bite 16......");
     player.play(16);
     break;
   case 'G':
     Serial.println("Playing Sounds bite 17......");
     player.play(17);
     break;
   case 'H':
     Serial.println("Playing Sounds bite 18......");
     player.play(18);
     break;
   case 'I':
     Serial.println("Playing Sounds bite 19......");
     player.play(19);
     break;
   case 'J':
     Serial.println("Playing Sounds bite 20......");
     player.play(20);
     break;
   case 'K':
     Serial.println("Playing Sounds bite 21......");
     player.play(21);
     break;
   case 'L':
     Serial.println("Playing Sounds bite 22......");
     player.play(22);
     break;
   case 'M':
     Serial.println("Playing Sounds bite 23......");
     player.play(23);
     break;
   case 'a':
     Serial.println("setting volume 5......");
     player.volume(5);
     break;
   case 'b':
     Serial.println("setting volume 10......");
     player.volume(10);
     break;
   case 'c':
     Serial.println("setting volume 15......");
     player.volume(15);
     break;
   case 'd':
     Serial.println("setting volume 20......");
     player.volume(20);
     break;
   case 'e':
     Serial.println("setting volume 25......");
     player.volume(25);
     break;
   case 'f':
     Serial.println("setting volume 30......");
     player.volume(30);
     break;
   case 'g':
     Serial.println("setting volume OFF......");
     player.volume(0);
     break;
   case 'h':
     Serial.println("setting eye color YELLOW......");
     eyeson(1);
     break;
   case 'i':
     Serial.println("setting eye color RED......");
     eyeson(2);
     break;
   case 'j':
     Serial.println("setting eye color GREEN......");
     eyeson(3);
     break;
   case 'k':
     Serial.println("setting eye color BLUE......");
     eyeson(4);
     break;
   case 'l':
     Serial.println("setting eye color DEEP PINK......");
     eyeson(5);
     break;
   case 'm':
     Serial.println("setting eye color AQUA......");
     eyeson(6);
     break;

   case 'n':
     Serial.println("setting eye color RES/WHITE/BLUE......");
     eyeson(7);
     break;
   case 'o':
     Serial.println("setting eye color ......");
     eyeson(8);
     break;
   case 'p':
     Serial.println("shutting eyes off ......");
     eyeson(9);
     break;
   default:
     Serial.println("Opps...recieved invalid MQTT command......");
     player.play(17);
     break;
  }
} 


void checkIn()
{
  client.publish(USER_MQTT_CLIENT_NAME"/checkIn","OK"); 
}

/*
void eyepixelr (int pixel, int aa, int bb, int cc) {
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(pixel, pixelsr.Color(aa, bb, cc));
     pixelsr.show();
  
}

void eyepixell (int pixel, int aa, int bb, int cc) {
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(pixel, pixelsl.Color(aa, bb, cc));
     pixelsl.show();
  
}
*/
void eyeson(int eyecolor) {

   switch (eyecolor) {
   case 1:  // YELLOW
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(255, 255, 0));
     pixelsr.setPixelColor(1, pixelsr.Color(255, 255, 0));
     pixelsr.setPixelColor(2, pixelsr.Color(255, 255, 0));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(255, 255, 0));
     pixelsl.setPixelColor(1, pixelsl.Color(255, 255, 0));
     pixelsl.setPixelColor(2, pixelsl.Color(255, 255, 0));
     pixelsl.show();  
     break;
   case 2:  // RED
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(255, 0, 0));
     pixelsr.setPixelColor(1, pixelsr.Color(255, 0, 0));
     pixelsr.setPixelColor(2, pixelsr.Color(255, 0, 0));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(255, 0, 0));
     pixelsl.setPixelColor(1, pixelsl.Color(255, 0, 0));
     pixelsl.setPixelColor(2, pixelsl.Color(255, 0, 0));
     pixelsl.show();  
     break;
   case 3:  // GREEN
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(0, 255, 0));
     pixelsr.setPixelColor(1, pixelsr.Color(0, 255, 0));
     pixelsr.setPixelColor(2, pixelsr.Color(0, 255, 0));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(0, 255, 0));
     pixelsl.setPixelColor(1, pixelsl.Color(0, 255, 0));
     pixelsl.setPixelColor(2, pixelsl.Color(0, 255, 0));
     pixelsl.show();  
     break;
    case 4:  // BLUE
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(0, 0, 255));
     pixelsr.setPixelColor(1, pixelsr.Color(0, 0, 255));
     pixelsr.setPixelColor(2, pixelsr.Color(0, 0, 255));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(0, 0, 255));
     pixelsl.setPixelColor(1, pixelsl.Color(0, 0, 255));
     pixelsl.setPixelColor(2, pixelsl.Color(0, 0, 255));
     pixelsl.show();  
     break;
    case 5:  // DEEP PINK
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(255, 20, 147));
     pixelsr.setPixelColor(1, pixelsr.Color(255, 20, 147));
     pixelsr.setPixelColor(2, pixelsr.Color(255, 20, 147));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(255, 20, 147));
     pixelsl.setPixelColor(1, pixelsl.Color(255, 20, 147));
     pixelsl.setPixelColor(2, pixelsl.Color(255, 20, 147));
     pixelsl.show();  
     break;
    case 6:  // AQUA
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(0, 255, 255));
     pixelsr.setPixelColor(1, pixelsr.Color(0, 255, 255));
     pixelsr.setPixelColor(2, pixelsr.Color(0, 255, 255));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(0, 255, 255));
     pixelsl.setPixelColor(1, pixelsl.Color(0, 255, 255));
     pixelsl.setPixelColor(2, pixelsl.Color(0, 255, 255));
     pixelsl.show();  
     break;
    case 7:  // RED WHILE BLUE
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(255, 0, 0));
     pixelsr.setPixelColor(1, pixelsr.Color(255, 255, 255));
     pixelsr.setPixelColor(2, pixelsr.Color(0, 0, 255));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(255, 0, 0));
     pixelsl.setPixelColor(1, pixelsl.Color(255, 255, 255));
     pixelsl.setPixelColor(2, pixelsl.Color(0, 0, 255));
     pixelsl.show();  
     break;
    case 8: //CIRCLE EYES
     chase(pixelsr.Color(0, 255, 0));
     chase(pixelsl.Color(0, 255, 0));
     break; 
    case 9: //CLOSE EYES
     pixelsr.clear();
     pixelsr.setBrightness(110);
     pixelsr.setPixelColor(0, pixelsr.Color(0, 0, 0));
     pixelsr.setPixelColor(1, pixelsr.Color(0, 0, 0));
     pixelsr.setPixelColor(2, pixelsr.Color(0, 0, 0));
     pixelsr.show();
     pixelsl.clear();
     pixelsl.setBrightness(110);
     pixelsl.setPixelColor(0, pixelsl.Color(0, 0, 0));
     pixelsl.setPixelColor(1, pixelsl.Color(0, 0, 0));
     pixelsl.setPixelColor(2, pixelsl.Color(0, 0, 0));
     pixelsl.show();  
     break; 
    default:
     break; 
   }
}

/*
void sensorPub(char *chipname,int n,int v,int k)

// BUILD CHAR ARRAY TO PUBLISH

{
  StaticJsonDocument<256> doc;
  char addbuf[6];
  char addresstopic[50];

  strcpy(addresstopic,USER_MQTT_CLIENT_NAME);
  strcat(addresstopic,"/sensorout/");
  strcpy(addbuf,chipname);
  strcat(addbuf, portName[n]);
  strcat(addresstopic,addbuf); //make a topic with the address name
  doc["chip"] = chipname;
  doc["port"] = n;
  doc["address"] = addbuf;
  doc["state"] = stateme[v];

  
  char mybuffer[256];
  serializeJson(doc, mybuffer);
  client.publish(addresstopic, mybuffer);// THIS IS WHERE TO PUBLISH THE SENSOR DATA
  char buf[50];
  const char *second = " - ";
  strcpy(buf,chipname);
  strcat(buf,second);
  strcat(buf, portName[n]);
  strcat(buf,second);
  strcat(buf, stateme[v]); 
  report_to_display(buf,addresstopic,k); // THIS IS WHERE TO PRINT TO SCREEN TOPIC AND CHANGE
  Serial.println(buf);  // THIS IS SHOW IN SERIAL THE SENSOR DATA
 

}
*/

void setup_eyes_sound() {
  // initialize LED eyes
  pixelsr.begin();
  pixelsl.begin();
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
  player.begin(softwareSerial);
  // Start communication with DFPlayer Mini
  // set volume maximum (0 to 30).
  player.volume(30);
 
 }


//Run once setup
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  if (!client.connected()) 
  {
    reconnect();
  }
  setup_eyes_sound();
  Serial.println ("States read...ready for action!");
  ArduinoOTA.setHostname(USER_MQTT_CLIENT_NAME);
  ArduinoOTA.begin(); 
//  delay(10);
}


void loop() 

{
  if (!client.connected()) 
  {
    reconnect();
  }
// HERE NEEDS TO GO A ROUTINE FOR CHECKING SENSOR DATA 
  client.loop(); 
  ArduinoOTA.handle();
//  timer.run();
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<3+3; i++) {
      pixelsr.setPixelColor(i  , c); // Draw new pixel
      pixelsr.setPixelColor(i-3, 0); // Erase pixel a few steps back
      pixelsr.show();
      pixelsl.setPixelColor(i  , c); // Draw new pixel
      pixelsl.setPixelColor(i-3, 0); // Erase pixel a few steps back
      pixelsl.show();
      
      delay(500);
  }
}
