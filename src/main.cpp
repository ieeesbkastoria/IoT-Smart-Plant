#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

const int Spin = 20;//moisture sensor pin
DHT dht(26, DHT11);
int moisture_lvl, sensor_analog = 4095;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Spin, INPUT);
  
  
  dht.begin();
  delay(2000);

  Serial.begin(115200);
  
}

void loop() {
  //moisture sensor
  sensor_analog = analogRead(Spin);
  moisture_lvl = (100 - ( ( sensor_analog / 4095) * 100) );// formula presentage
  Serial.print("Moisture = ");
  Serial.print(moisture_lvl);
  Serial.print("%");
  delay(1000);

  //Temperature-Humidity sensor
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" % ");
  delay(2000);
  
}

