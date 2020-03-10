int in_sensor =7;
int  out_sensor =8;
int buzzer =9;


int leitura_tcrt5000(int in_sensor,int out_sensor){
int status_bit = 0;
digitalWrite(out_sensor,HIGH);delay(50); 
status_bit = digitalRead(out_sensor);delay(50);
digitalWrite(out_sensor,LOW);
  return status_bit;
}

void setup() {
pinMode(in_sensor,INPUT);
pinMode(out_sensor,OUTPUT);digitalWrite(out_sensor,LOW);
}

void loop() {
if(leitura_tcrt5000(in_sensor,out_sensor)){
  digitalWrite(buzzer,HIGH);
  delay(10);
}
  digitalWrite(buzzer,LOW);
}
