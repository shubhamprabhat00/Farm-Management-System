#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define dht_dpin 9
DHT dht(dht_dpin,DHTTYPE);


SoftwareSerial sw(3,2);// RX/TX
#define TRIGGERPIN 6
#define ECHOPIN 7


const int moisture_pin = A0;  /* Soil moisture sensor O/P pin */

int relayPin = 10;// Connected to relay


void setup() 
{
  dht.begin(9600);
  Serial.begin(9600);
  sw.begin(9600);//the baud rate at which data is sent should be same in node mcu receiving end
  pinMode(TRIGGERPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(moisture_pin, OUTPUT);
  delay(1000);//Wait before accessing Sensor
}
/*sends hello to node mcu after each 1sec*/
void loop() 
{
  //ultra sonic
   long duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("Distance is: ");
  Serial.print(distance);
  Serial.println("Cm");
  delay(1000);

    
//ultra sonic end
//dht 11 start 
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(1000);
/////////////////////////////////////////
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(moisture_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
  delay(1000);

   /////////////////////////////////////
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["data1"] = h;
    root["data2"] =t;
    root["data3"]=distance;
    root["data4"]=moisture_percentage;
     if(sw.available()>0)
      {
       root.printTo(sw);
       Serial.println("datasent");
      }
     Serial.println("");
      delay(1000);
      
      
}
