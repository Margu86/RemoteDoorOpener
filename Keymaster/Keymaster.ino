#include <ESP8266Ping.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP UDP;

#define WIFI_SSID "YOURNETWORK_SSID"
#define WIFI_PASS "YOUREXTREMLYSAVEPASSWORD"
#define UDP_PORT 7001
const int buttonPin = D3;
const int ledPin = BUILTIN_LED;
int buttonState = 1;
bool pressed = false;
IPAddress ip(10,0,0,3); // IP of the opening button, use a different IP for every Remote-Controll
IPAddress gateway(10,0,0,1);
IPAddress subnet(255,255,255,0);
IPAddress gatekeeper(10,0,0,2); // IP of the door-opener (Gatekeeper)

void setup() {
  Serial.begin(115200);
  Serial.print("Status: ");
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, gateway, subnet);
  WiFi.hostname("KeyMaster1");
  WiFi.begin(WIFI_SSID, WIFI_PASS); //connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.println('.');
  }
  Serial.println(WiFi.localIP());
  pinMode(buttonPin, INPUT);
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, !buttonState);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if(!buttonState == HIGH){
    pressed = true;    
  }
  if(pressed && !buttonState == LOW){
    Serial.println("Button released, sending command");
    sendCommand(1);
    pressed = false;
  }
}

void sendCommand(int com){
  UDP.beginPacket(gatekeeper, UDP_PORT);
  UDP.write((char *)&com,8);
  UDP.endPacket();
}
