/*A resolução do esp 32 tem 12 bits de ADC (leitura analogica) e 3.3 V*/


int pot = 36;
float tens;

void setup() {
pinMode(pot,INPUT);
Serial.begin(115200);

}

void loop() {

tens = ((analogRead(pot)/4095.0)*3.3);
Serial.println(tens);
delay(100);
}
