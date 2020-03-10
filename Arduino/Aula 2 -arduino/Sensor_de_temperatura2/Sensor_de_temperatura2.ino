#define LM35 A0
float temp = 0;
void setup() {
Serial.begin(9600); 

}

void loop() {
temp = analogRead(LM35); temp=map(temp,0,1023,0,5000); temp = temp/10; //MAP - converte proporcionalmente, porém so retorna inteiros. Por isso utilizei a escala 5000 mV. No final divido por 10, pois ficaria 10 vezes maior (10 mV /Grau) 
Serial.println(temp);
delay(500);  
}
