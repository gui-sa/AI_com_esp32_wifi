int blue = 23;
int red = 33;
int green = 25;

void setup() {
pinMode(blue,OUTPUT); 
pinMode(red,OUTPUT); 
pinMode(green,OUTPUT); 

}

void loop() {
digitalWrite(blue,LOW);
delay(1000);
digitalWrite(blue,HIGH);
delay(1000);

digitalWrite(red,LOW);
delay(1000);
digitalWrite(red,HIGH);
delay(1000);

digitalWrite(green,LOW);
delay(1000);
digitalWrite(green,HIGH);
delay(1000);

digitalWrite(red,LOW);
digitalWrite(green,LOW);
digitalWrite(blue,LOW);
delay(1000);
digitalWrite(red,HIGH);
digitalWrite(blue,HIGH);
digitalWrite(green,HIGH);
delay(1000);


}
