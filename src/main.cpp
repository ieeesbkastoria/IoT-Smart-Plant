#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <cmath> //Δεν ξερω που το θελει

  int r = 1;

//wifi  and MQTT settings
const char* ssid = "IEEE Lab"; //wifi name here
const char* password = "IEEE@2025"; // wifi password here

const char* mqtt_server = "192.168.69.2";
const int mqtt_port = 1880;

/*---------------------------------------------------*/

DHT dht(11, DHT11);//DHT input on pin 11(humidity and temperature)

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
}// When connected doesn't print enything

void setup()
{
  Serial.begin(57600);

  InitWiFi();
  Serial.print("RRSI: ");
  Serial.print(WiFi.RSSI());
}

void loop()
{
  int sensor_analog = analogRead(1);//moisture lvl input pin = 1
  int dry_valeu = 1500;
  int wet_valeu = 4095;
  float humidity = 0;
  float temperature = 0;

/*--------------------------------------------------------------------*/
//moisture lvl read and precentage
 int moisture_lvl = map(sensor_analog, dry_valeu, wet_valeu, 0, 100);

  moisture_lvl = constrain(moisture_lvl, 0, 100);

/*--------------------------------------------------------------------*/
//print data
  Serial.print("This is the map algorithm:");
  Serial.print(moisture_lvl);
  Serial.print("%\n" );
  Serial.print("\n");
  //delay(1500);
  
  
/*---------------------------------------------------------------------*/
//temperature and humidity recording
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

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
  delay(5000);

/*-------------------------------------------------------------------*/
  r = r * 2;
}