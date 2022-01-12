#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP UDP;

const char ssid[] = "YOURNETWORK_SSID";
const char password[] = "YOUREXTREMLYSAVEPASSWORD";
const int checkDelay = 5000;
const int ledPin = BUILTIN_LED;
const int relayPin = D1;
unsigned long lastChecked;
IPAddress ip(10,0,0,2); // IP of the door-opener (this device)
IPAddress gateway(10,0,0,1);
IPAddress subnet(10,0,0,0);

int myint;

void setup() {
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password); // Connect to the network
  Serial.println(WiFi.localIP());
  UDP.begin(7001);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 0);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1);
}

void loop() {
  int packetSize = UDP.parsePacket();
  if (packetSize){
    Serial.println("Received some Data over UDP ...");
    UDP.read((char *)&myint, 8);
  }
  int opCode = myint;
  if(opCode == 1){
    opCode = 0;
    myint = 0;
    openDoor();
  }
}

void openDoor(){
  Serial.println("Open the Door ...");
  digitalWrite(ledPin, 0);
  digitalWrite(relayPin, 1);
  delay(15000);
  digitalWrite(ledPin, 1);
  digitalWrite(relayPin, 0);
}
