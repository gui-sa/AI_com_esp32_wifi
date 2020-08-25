/*
 * O objetivo deste codigo é enviar e receber dados via rede.
 * A ideia inicial é apenas enviar uma quantia em Bytes de uma informação.
 * https://www.usinainfo.com.br/blog/esp32-wifi-comunicacao-com-a-internet/
 * https://randomnerdtutorials.com/esp32-client-server-wi-fi/
 */

  
#include <WiFi.h> //Lib que lida com o processo wifi 


#define led_interno 2

//=================================Dados ligados a sua rede de internet
const char* ssid = "Raiolux";  
const char* password =  "raiolux1020";

const char* serverNameTemp = "http://192.168.4.1/temperature";





void sinal_led(int pin, int times_sec){
  int del = 1000/(times_sec*2);
  digitalWrite(led_interno,HIGH);
  delay(del);
  digitalWrite(led_interno,LOW);
  delay(del);
}




 
void setup(){

  
  pinMode(led_interno,OUTPUT);
  WiFi.begin(ssid, password);//Conecta o wifi seguindo estes dados
  
  while (WiFi.status() != WL_CONNECTED) {
    sinal_led(led_interno,5);
  }
  digitalWrite(led_interno,HIGH);
  
  WiFi.mode(WIFI_MODE_STA); //cliente wireless
}

 
void loop(){

}
