#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

DHT dht(11,DHT11);

float Sum = 0;
int t = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(1, INPUT);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  int sensor_analog;
  float dryValeu = 1500.0;                          //here we have the value when it's completly dry
  float wetValeu = 4095.0;                         //here we have the value when it's completly wet
  float value = wetValeu - dryValeu;

  sensor_analog = analogRead(1);
  Serial.print("This is the output of the sensor analog:");
  Serial.print(sensor_analog);
  Serial.print("\n");
  delay(3000);
  
   /*---------------------------------------------------------------------------------------------------*/

  // Code with map, here we take the analogRead and proportionly to where it is
  // between the dry and wet value you get the persentance in humidity
  int moisture_lvl = map(sensor_analog, dryValeu, wetValeu, 0, 100);

  //the constrain keeps the output value between the limmits. Our limmits are 0 - 100
  moisture_lvl = constrain(moisture_lvl, 0, 100);
  Serial.print("This is the map algorithm:");
  Serial.print(moisture_lvl);
  Serial.print("%\n" );
  delay(3000);

  

   /*---------------------------------------------------------------------------------------------------*/

  float Tiggos = (sensor_analog - dryValeu) / (wetValeu - dryValeu) * 100.0;
  // Περιορισμός μεταξύ 0 και 100
  if (Tiggos > 100.0) Tiggos = 100.0;
  if (Tiggos < 0.0) Tiggos = 0.0;
  Serial.print("This is Tiggos formula:");
  Serial.print(Tiggos);
  Serial.print("%\n");
  
  /*-------------------------------------Summary----------------------------------------------------------*/

  Sum = Sum + sensor_analog;
  t = t + 1;

  if (t >= 30)
  {
    Serial.print("Summary:");
    float Sum1;
    Sum1 = Sum / t;
    int m = map(Sum1, dryValeu, wetValeu, 0, 100);
    m = constrain(moisture_lvl, 0, 100);
    Serial.print(m);
    Serial.print("\n");

    t = 0;
  }

  Serial.print("Loop:");
  Serial.print(t);
  Serial.print("\n");

  /*--------------------Humidity sensor and Temperature sensor-----------------------------------------*/
  float humidity;
  float temperature;
  Serial.print("\n");

  temperature = dht.readTemperature();//from library
  humidity = dht.readHumidity();

  if(isnan(humidity) || isnan(temperature))//error on reading
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
}