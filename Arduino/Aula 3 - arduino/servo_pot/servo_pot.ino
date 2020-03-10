#include <Servo.h>
#define pot A0
#define servo 3

Servo servo1;       //Localizando objeto

int leitura;        //Variavel global para realizar leituram

void setup() {
servo1.attach(servo);  //o servo 1 esta localizado na porta 3

}

void loop() {
leitura = analogRead(pot);leitura = map(leitura,0,1023,0,180);  //Captura o valor do potenciometro e converte para graus
servo1.write(leitura); //Escreve na instancia servo1  a angulação desejada
  
}
