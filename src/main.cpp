#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <cmath>
#include <cstring>

//wifi  and MQTT settings
const char* ssid = "..."; //wifi name here
const char* password = "..."; // wifi password here

const char* mqtt_server = "192.168.69.2";

const int mqtt_port = 1883;


/*---------------------------------------------------*/
//DHT input on pin 11(humidity and temperature)
DHT dht(11, DHT11);

#define relay 14

WiFiClient espClient;
PubSubClient client(espClient);


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
  dht.begin();

  pinMode(relay, OUTPUT);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  InitWiFi();
  Serial.print("RRSI: ");
  Serial.print(WiFi.RSSI());
  
}

void loop()
{

  digitalWrite(relay, LOW);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  /*if (WiFi.status() != WL_CONNECTED)
  {
    InitWiFi();
  }*/

/*--------------------------------------------------------------------*/

  //moisture level input pin = 1
  int sensor_analog = analogRead(1);
  int dry_valeu = 1500;
  int wet_valeu = 4095;
  float humidity;
  float temperature;

/*--------------------------------------------------------------------*/
//moisture level precentage
 int moisture_lvl = map(sensor_analog, dry_valeu, wet_valeu, 0, 100);

  moisture_lvl = constrain(moisture_lvl, 0, 100);

  char soilString[10];

  dtostrf(moisture_lvl, 1, 2,soilString);

/*--------------------------------------------------------------------*/
 
 //Serial.print(moisture_lvl);
 //Serial.print("%\n" );
 //Serial.print("\n");
  
  
/*-------------------------------RELAY-----------------------------------*/
  if(moisture_lvl <= 15)
  {
    digitalWrite(relay,HIGH);
    delay(6000);
  }


//temperature and humidity readings
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  char humString[10]; //use for both humidity and temperature

  if(std::isnan(humidity) || std::isnan(temperature))
  {
    Serial.print("Error data reading \n");
  }
  else
  {

    //Serial.print("Temperature: ");
    //Serial.print(temperature);
    //Serial.print("C, ");
    //Serial.print("\n");
    //Serial.print("Humidity: ");
    //Serial.print(humidity);
    //Serial.print("%");
    //Serial.print("\n");

    dtostrf(humidity, 1, 2, humString);
    client.publish("hum", humString);//hum
    dtostrf(temperature, 1, 2, humString);
    client.publish("temp", humString);//temp
    client.publish("Soil", soilString);
  }
  
  delay(5000);
}
