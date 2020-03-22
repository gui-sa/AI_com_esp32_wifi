#define pot A0
#define led 3

int leitura;

void setup() {
pinMode(led,OUTPUT);

}

void loop() {
leitura = analogRead(pot); leitura = map(leitura,0,1023,0,255);  //Leitura e conversao
analogWrite(led,leitura);
delay(50);
}
