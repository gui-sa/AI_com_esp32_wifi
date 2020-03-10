#include <Ultrasonic.h>

#define echo 7
#define trigger 8

Ultrasonic zezim(trigger,echo);
float dist;

void setup() {
 Serial.begin(9600);
}

void loop() {
 dist = zezim.distanceRead();  //Em centimetros
 Serial.print("Distância: "); Serial.println(dist);
 delay(200);
}

