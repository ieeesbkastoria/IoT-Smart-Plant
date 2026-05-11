#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

static int randd(int x){

 int rand = 10;
 int r = 0;

 while( r < rand )
 {
   for(int r = 0; r < rand; r+2)
   {
     rand *= 2;
     x = rand+r;
   }
   
 }

 return x;
}