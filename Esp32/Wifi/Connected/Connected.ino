#include "WiFi.h"
 
const char* ssid = "RaioLuxTintas";//Nome da rede
const char* password =  "raiolux1020";//Senha da rede
 
void setup() {
 
  Serial.begin(115200);//Enable do serial do arduino IDE
 
  WiFi.begin(ssid, password);//iniciando a rede de nome tal com a senha tal
  
//WiFi.status returns:
//WL_CONNECTED: assigned when connected to a WiFi network;
//WL_NO_SHIELD: assigned when no WiFi shield is present;
//WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
//WL_NO_SSID_AVAIL: assigned when no SSID are available;
//WL_SCAN_COMPLETED: assigned when the scan networks is completed;
//WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
//WL_CONNECTION_LOST: assigned when the connection is lost;
//WL_DISCONNECTED: assigned when disconnected from a network;
 
  while (WiFi.status() != WL_CONNECTED) {//Enquanto ele nao se conecta, repita a escrita "conectando.."
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");//Se saiu do loop, significa que ele se conectou!
 
}
 
void loop() {
  //Faz nada
  
  }
