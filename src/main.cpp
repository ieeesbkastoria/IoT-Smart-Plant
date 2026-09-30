#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
//#include <string.h>
#include <cmath> //Δεν ξερω που το θελει

//wifi  and MQTT settings
const char* ssid = "IEEE Lab"; //wifi name here
const char* password = "IEEE@2025"; // wifi password here

const char* mqtt_server = "192.168.69.2";
<<<<<<< HEAD
const int mqtt_port = 1883;// or 1880
=======
>>>>>>> 511e6db (προστεθηκε node-red)

/*---------------------------------------------------*/

DHT dht(11, DHT11);//DHT input on pin 11(humidity and temperature)

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;


void InitWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("Connected");
}// When connected doesn't print enything

void callback(char* topic, byte* message, unsigned int lenth)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i<lenth; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  
  Serial.println();
}

void reconnect() {
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("espClient"))
    {
      Serial.println("connected");
      
      client.subscribe("esp/output");
    } 
    else
    {
      Serial.print("failed, rc= ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");//delay 5 seconds
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(200);

  client.setServer(mqtt_server, 1883);//1880
  client.setCallback(callback);

<<<<<<< HEAD
  client.setServer(mqtt_server, 1883);
=======
>>>>>>> 511e6db (προστεθηκε node-red)

  InitWiFi();
  Serial.print("RRSI: ");
  Serial.print(WiFi.RSSI());
}

void reconnect(){
  while (!client.connected()){
    Serial.print("Atempting MQTT connection...");
    if (client.connect("espClient")){
       Serial.print("Succesfully connected!");
       //subscribe
       client.subscribe("ESP32/output");
     } else{
      Serial.print("Failed, rc = ");
      Serial.print(client.state());
      Serial.print("try again in 5s ");
      delay(5000);
     }
  }

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(!client.connected()){
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) 
  {
    lastMsg = now;
  }

  int sensor_analog = analogRead(1);//moisture lvl input pin = 1
  int dry_valeu = 1500;
  int wet_valeu = 4095;
  float humidity = 0;
  float temperature = 0;

/*--------------------------------------------------------------------*/
//moisture lvl read and precentage
 int moisture_lvl = map(sensor_analog, dry_valeu, wet_valeu, 0, 100);

  moisture_lvl = constrain(moisture_lvl, 0, 100);

  char soilString[5];

  dtostrf(moisture_lvl, 1, 2,soilString);

/*--------------------------------------------------------------------*/
//print data
  //Serial.print("This is the map algorithm:");
  Serial.print(moisture_lvl);
  Serial.print("%\n" );
  Serial.print("\n");
  //delay(1500);
  
  
/*---------------------------------------------------------------------*/
//temperature and humidity recording
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  char humString[5];
  char tempString[5];

  if(std::isnan(humidity) || std::isnan(temperature))
  {
    Serial.print("Error data reading \n");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, ");
  Serial.print("\n");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%");
  Serial.print("\n");

  dtostrf(temperature, 1, 4, tempString);
  dtostrf(humidity, 1, 2, humString);
  client.publish("hum", humString);//hum
  client.publish("temper", tempString);//temp
  client.publish("Soil", soilString);
  
  delay(5000);
}