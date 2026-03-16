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

  int sensor_analog;
  float dryValeu = 4095.0;                          //here we have the value when it's completly dry
  float wetValeu = 1200.0;                         //here we have the value when it's completly wet
  float value = wetValeu - dryValeu;

  sensor_analog = analogRead(1);
  Serial.print("This is the output of the sensor analog:");
  Serial.print(sensor_analog);
  Serial.print("\n");
  delay(3000);
  
/*---------------------------------------------------------------------------------------------------*/

  //code we did with Christos
  Serial.print("Thanasis Algorithm: ");
  float thanasis = sensor_analog/32.5;
  Serial.print(thanasis);
  Serial.print("%\n");
  delay(3000);

  /*---------------------------------------------------------------------------------------------------*/

  //Fist type of algorithm we found to take humidity
  int First = (100 - ( ( sensor_analog / 4095.0) * 100) );
  Serial.print("This is the first:");
  Serial.print(First);
  Serial.print("\n");
  delay(3000);

  /*---------------------------------------------------------------------------------------------------*/

  // Code with map, here we take the analogRead and proportionly to where it is
  // between the dry and wet value you get the persentance in humidity
  int moisture_lvl = map(sensor_analog, dryValeu, wetValeu, 0, 100);

  //the constrain keeps the output value between the limmits. Our limmits are 0 - 100
  //moisture_lvl = constrain(moisture_lvl, 0, 100);
  Serial.print("This is the map algorithm:");
  Serial.print(moisture_lvl);
  Serial.print("\n");
  delay(3000);

  /*---------------------------------------------------------------------------------------------------*/

  //Παραλαγή του πραγματικόυ τήπου για την υγρασία
  int Percentage = 100 - ( ( (sensor_analog - wetValeu) * 100L ) / (dryValeu - wetValeu) );
  Serial.print("This is the first Percentage:");
  Serial.print(Percentage);
  Serial.print("\n");
  delay(3000);

  /*---------------------------------------------------------------------------------------------------*/

  //Δεύτερη παραλαγή του τύπου για την υγρασία
  int percentage = 100 - ( ( (sensor_analog - wetValeu) / (float)(dryValeu - wetValeu) ) * 100 );
  Serial.print("This is the second Persentage:");
  Serial.print(percentage);
  Serial.print("\n");
  delay(3000);

  /*---------------------------------------------------------------------------------------------------*/

  float Tiggos = (sensor_analog - dryValeu) / (wetValeu - dryValeu) * 100.0;
  // Περιορισμός μεταξύ 0 και 100
  if (Tiggos > 100.0) Tiggos = 100.0;
  if (Tiggos < 0.0) Tiggos = 0.0;
  Serial.print("This is Tiggos formula:");
  Serial.print(Tiggos);
  Serial.print("\n");

}