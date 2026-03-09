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
  //int dryValeu = 4095;                             //wehe we have the value when it's completly dry
  //int wetValeu = 1000;                             //wehe we have the value when it's completly wet
  //float value = wetValue - dryValue;

  sensor_analog = analogRead(1);
  Serial.print(sensor_analog);
  Serial.print("\n");
  delay(5000);
  
/*---------------------------------------------------------------------------------------------------*/

  //code we did with Christos
  Serial.print("Thanasis Algorithm: ");
  float thanasis = sensor_analog/32.5;
  Serial.print(thanasis);
  Serial.print("%\n");
  delay(5000);

  /*---------------------------------------------------------------------------------------------------*/

  //Fist type of algorithm we found to take humidity
  int First = (100 - ( ( sensor_analog / 4095.0) * 100) );
  Serial.print("This is the first:");
  Serial.print(First , "%\n");

  /*---------------------------------------------------------------------------------------------------*/

  // code with map, here we take the analog read and proportionly to where it is
  // between the dry and wet value you get the persentance in humidity
  int moisture_lvl = map(sensor_analog, dryValue, wetValue, 0, 100);

  //the constrain keeps the output value between the limmits 0 - 100
  int moisture_lvl = constrain(moisture_lvl, 0, 100);
  Serial.print("This is the map algorithm:")
  Serial.print(moisture ,"%\n" );

  /*---------------------------------------------------------------------------------------------------*/

  //Παραλαγή του πραγματικόυ τήπου για την υγρασία
  int Percentage1 = 100 - ( ( (sensor_analog - wetValue) * 100L ) / (dryValue - wetValue) );
  Serial.print("This is the first Percentage:");
  Serial.print(Percentage1 , "%\n");

  /*---------------------------------------------------------------------------------------------------*/

  //Δεύτερη παραλαγή του τύπου για την υγρασία
  int Percentage2 = 100 - ( ( (sensor_analog - wetValue) / (float)(dryValue - wetValue) ) * 100 );
  Serial.print("This is the second Persentage:");
  Serial.print(Percentage2 , "%\n");

}