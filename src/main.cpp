#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float moisture_lvl; 
  int sensor_analog;
  //int dryValeu = 4095;
  //int wetValeu = 1000;
  //float value = 4095 - 1000;

  sensor_analog = analogRead(1);
  Serial.print(sensor_analog);
  Serial.print("\n");
  delay(5000);
  Serial.print("Thanasis Algorithm: ");
  float thanasis = sensor_analog/32.5;
  Serial.print(thanasis);
  Serial.print("%");
  Serial.print("\n");
  delay(5000);
}