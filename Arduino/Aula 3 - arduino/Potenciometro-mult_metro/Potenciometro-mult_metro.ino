#define pot A0

float tensao;

void setup() {
Serial.begin(9600);


}

void loop() {
tensao = analogRead(pot); tensao= (tensao*5.0)/1023.0;
Serial.println(tensao);
delay(200);
}
