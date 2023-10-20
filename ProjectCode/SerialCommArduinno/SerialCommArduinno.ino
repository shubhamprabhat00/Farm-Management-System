#include <SoftwareSerial.h>
#include <ArduinoJson.h>
/*#include "DHT.h"
#define DHTTYPE DHT11
#define dht_dpin A0
DHT dht(dht_dpin,DHTTYPE);*/
int mosture_pin=A1;
int mosture_output;
SoftwareSerial sw(3,2);// RX/TX
const int trig = 7;
const int echo = 8;
int dist,dur;
int output_value;  
void setup() 
{
  Serial.begin(9600);
  sw.begin(9600);//the baud rate at which data is sent should be same in node mcu receiving end
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  delay(1000);//Wait before accessing Sensor
}
/*sends hello to node mcu after each 1sec*/
void loop() 
{
  //ultra sonic
   digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
    dur=pulseIn(echo,HIGH);
    delayMicroseconds(10);
    dist=(dur*0.0340)/2;
    Serial.print("Distance in cm:");
    Serial.println(dist);

    
//ultra sonic end
//dht 11 start 
/*  int chk=DH.read11(A0);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print("Current Temperature: ");
    Serial.println(t);
    Serial.print("Current Humidity: ");
    Serial.println(h);*/

/////////////////////////////////////////
    mosture_output= analogRead(mosture_pin);
   mosture_output = map(mosture_output,1020,283,0,100);
   Serial.print("Mositure in percentage : ");
   Serial.print(mosture_output);
   Serial.println("%");

   /////////////////////////////////////
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["data1"] = dist;
    //root["data2"] =h;
    //root["data3"]=t;
    root["data4"]=mosture_output;
     if(sw.available()>0)
      {
       root.printTo(sw);
       Serial.println("datasent");
      }
     Serial.println("");
      delay(1000);
      
      
}
