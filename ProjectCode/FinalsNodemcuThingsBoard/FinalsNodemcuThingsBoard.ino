#include <SoftwareSerial.h>
#include <PubSubClient.h>
SoftwareSerial NodeSerial(2, 12);// RX, TX
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "minipogo"
#define WIFI_PASSWORD "rummiporn"

#define TOKEN "Q8e3eO45nuLvkBmBITES"

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() 
{
 
  Serial.begin(115200);
  NodeSerial.begin(9600);//the baud rate at which data is sent should be same in nodemcu receiving end
  while(!Serial) continue;
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
}

void loop() 
{
delay(1000);
    if ( !client.connected() ) {
    reconnect();
  }
  
 Serial.println("\n \n \nData Coming from Arduino........."); 
   StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(NodeSerial);
    if (root == JsonObject::invalid())
     return;

  Serial.print("Current Humidity :");
  int data1=root["data1"];
  Serial.println(data1);

  Serial.print("Current Temperature :");
  int data2=root["data2"];
  Serial.print(data2);
  Serial.println("C");

  Serial.print("Distance :");
  int data3=root["data3"];
  Serial.println(data3);
 
  Serial.print("Moisture in soil :");
  int data4=root["data4"];
  Serial.print(data4);
  Serial.println("%\n");



 if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
//      Serial.println("Collecting temperature data.");
  delay(500);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  float h = data1;
  
  // Read temperature as Celsius (the default)
  float t = data2;
  
  //read distance
  float d = data3;

  //Reading Moisture in percentage
  float m = data4;

  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(d) || isnan(m)) {
    Serial.println("Failed to read from Arduino sensor!");
    return;
  }

  String temperature = String(t);
  String humidity = String(h);
  String distance = String(d);
  String moisture = String(m);

  // Just debug messages
  delay(500);
  Serial.println( "\n \nSending Data To ThingsBoard" );
  Serial.print(temperature); 
  Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"Temperature\":"; payload += temperature; payload += ",";
  payload += "\"Humidity\":"; payload += humidity; payload += ",";
  payload += "\"Distance\":"; payload += distance; payload += ",";
  payload += "\"Moisture\":"; payload += moisture; 
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );

    lastSend = millis();
  }

  client.loop();
  
//delay(500);
}

 ////////////////////////////ThingsBoard/////////////////////


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
 
}
 
