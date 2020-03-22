#define sensor A0

int valor;
void setup() {
  Serial.begin(9600);

}

void loop() {
valor = analogRead(sensor); Serial.println(valor);
delay(100) ;

}
