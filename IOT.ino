#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTPIN D4    
#define DHTTYPE DHT11  
 
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
// Update these with values suitable for your network.

const char* ssid = "Wael";
const char* password = "Wael";
const char* mqtt_server = "127.0.0.1:1883";


int Humidite=4;       
int Temperateur=5;
int pomp = 9;    
int pomp1 = 8;
int EAU=10;
int tsol;
int hsol;  


// Initialize wifi & PubSub client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("senseclient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void setup() {

    pinMode(Humidite, INPUT);  
    pinMode(pomp, OUTPUT);  
    pinMode(Temperateur, INPUT); 
    pinMode(EAU,INPUT);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}
unsigned long next_submission = 0;
void loop() {
  client.loop();
  if(millis()>next_submission){
    next_submission = millis() + 5000;
  if (!client.connected()) {
    reconnect();
  }
  DynamicJsonDocument doc(1024);
  JsonObject obj=doc.as<JsonObject>();
  float temp=dht.readTemperature();
  float humid=dht.readHumidity();
  doc["Temp"]= temp;
  doc["Humid"]= humid;
  char jsonStr[60];
  serializeJson(doc,jsonStr);
  Serial.print(jsonStr);
  Serial.println();
  client.publish("testing",jsonStr);

int EAU = digitalRead(10);
float tension = (float)(5* EAU/1023); 
Serial.print( "tension : ");
Serial.println(EAU); 
delay (1000); 
// 0  0,82 v 
// 25  1,51 v 
//50  1,70v 
// 75  1,77 v  
//100 1,86 v

if(tension < 1.86) {

  Serial.println("niveau a 0 ");
  digitalWrite(pomp1,HIGH);
  delay(1000);

}
else {

  Serial.println("niveau a 100 ");
  digitalWrite(pomp1,LOW);
  delay(1000);

}



  int hsol = digitalRead(Humidite);
  int tsol = digitalRead(Temperateur);
  int Nveau =digitalRead(EAU); 
  Serial.println("Sol humiditee est = "); 
  Serial.print(hsol);
  Serial.println("La Temperateur est = "); 
  Serial.print(tsol);
                    
if (tsol >30 && hsol > 500)
  {
    digitalWrite(pomp,HIGH);
    delay(1000);
  }
  else 
  {
    digitalWrite(pomp,LOW);
    delay(1000);
  }
 delay(6000);
}
