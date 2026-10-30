#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <cmath>
#include <cstring>
#include <secrets.h>

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
  WiFi.begin(SECRET_SSID, SECRET_PASS);
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

    //esp will be in this "while" until it will connect again 
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print("WiFi disconnected. Reconnecting to WIFI..");
      
      WiFi.disconnect();
      WiFi.begin();
      delay(5000);

      if(WiFi.status() == WL_CONNECTED)
      {
        Serial.print("WiFI Reconnected");
      }
    }

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


/*--------------------------------------------------------------------*/

  //moisture level input pin = 1
  int sensor_analog = analogRead(1);
  int dry_valeu = 1500;
  int wet_valeu = 4095;
  float humidity;
  float temperature;

/*---------------------------SOIL_MOISTURE_PERCENTAGE-----------------------------------------*/

 int moisture_lvl = map(sensor_analog, dry_valeu, wet_valeu, 0, 100);

  moisture_lvl = constrain(moisture_lvl, 0, 100);

  char soilString[10];

  dtostrf(moisture_lvl, 1, 2,soilString);
  
  
/*-------------------------------RELAY-----------------------------------*/
  if(moisture_lvl <= 15)
  {
    digitalWrite(relay,HIGH);
    delay(6000);
  }

/*----------------------------DHT_11------------------------------------*/

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  char humString[10]; //use for both humidity and temperature

  if(std::isnan(humidity) || std::isnan(temperature))
  {
    Serial.print("Error data reading \n");
  }
  else
  {

    //Here we sent data from sensors to Database 
    dtostrf(humidity, 1, 2, humString);
    client.publish("hum", humString);         //humidity value

    dtostrf(temperature, 1, 2, humString);
    client.publish("temp", humString);         //temperature value

    client.publish("Soil", soilString);        //soil humidity value
  }
  
  delay(5000);
}
