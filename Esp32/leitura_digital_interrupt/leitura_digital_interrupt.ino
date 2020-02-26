int white = 33;
int but = 36;
int blue = 25;


void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR
  digitalWrite(blue,!digitalRead(blue));
}

void setup() {
  
pinMode(white,OUTPUT);
pinMode(blue,OUTPUT);
pinMode(but,INPUT);
attachInterrupt(digitalPinToInterrupt(but),f_int1, RISING);
}

void loop() {

digitalWrite(white,1);
delay(5000);
digitalWrite(white,0);
delay(5000);
}
