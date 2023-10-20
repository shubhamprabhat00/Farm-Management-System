#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(2, 3);// RX, TX
#include <ArduinoJson.h>

void setup() 
{
 
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
  
  int data2=root["data2"];
  Serial.println(data2);

  int data3=root["data3"];
  Serial.println(data3);
  
  
 
  Serial.print("Moisture in soil is : ");
  int data4=root["data4"];
  Serial.print(data4);
  Serial.println("%");
 
}
 
