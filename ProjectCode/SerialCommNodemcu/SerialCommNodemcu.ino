
#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(2, 3);// RX, TX
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT11
#include <ESP8266HTTPClient.h>
#define dht_dpin 0
DHT dht(dht_dpin,DHTTYPE);
/*WiFiClient client;

String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;

HTTPClient http;*/

void setup() 
{
  /* WiFi.disconnect();
   WiFi.begin("surya","SURYA921");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);*/
  
//  dht.begin();
  Serial.begin(9600);
  NodeSerial.begin(9600);//the baud rate at which data is sent should be same in nodemcu receiving end
  while(!Serial) continue;
}

void loop() 
{
   Serial.println("i m in loop");
   StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(NodeSerial);
    if (root == JsonObject::invalid())
     return;
  int data1=root["data1"];
  Serial.println(data1);
  Serial.print("Current Temperature: ");
  //int data3=root["data3"];

 
  Serial.print("Moisture in soil is : ");
  int data4=root["data4"];
  Serial.print(data4);
  Serial.println("%");
    delay(1000);
}
 
