#include <SoftwareSerial.h>       //Funçao que que transforma outras portas em comunicaçao serial
SoftwareSerial bluetooth(10,11); //RX, TX

int led= 3;
int intensidade_led = 0;
void setup() {
  pinMode(led, OUTPUT);   //Muito importante
  digitalWrite(led, LOW);
  bluetooth.begin(9600);
}

void loop() {
  if (bluetooth.available() > 0) {
    int lerbt = bluetooth.read();
   if (lerbt == 'a') {
      while (!bluetooth.available());
        intensidade_led = bluetooth.read();
        analogWrite(led,intensidade_led);
      }
    if (lerbt == 'b') {
      digitalWrite(led, LOW);
    }
  }

}
