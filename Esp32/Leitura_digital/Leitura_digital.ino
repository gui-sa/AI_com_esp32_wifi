int led = 33;
int but = 36;


void setup() {
  
pinMode(led,OUTPUT);
pinMode(but,INPUT);
}

void loop() {

  if(digitalRead(but)==1)
  {
     digitalWrite(led,!digitalRead(led));
     delay(200);
  }


}
