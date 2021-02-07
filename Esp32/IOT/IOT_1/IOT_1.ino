/*
 * O objetivo deste codigo é enviar e receber dados via rede.
 * A ideia inicial é apenas enviar uma quantia em Bytes de uma informação.
 * https://www.usinainfo.com.br/blog/esp32-wifi-comunicacao-com-a-internet/
 * https://randomnerdtutorials.com/esp32-client-server-wi-fi/
 * https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
 * Uma pagina na web local sera criada com a frase "Hello World", um local para colocar qlqr string (com limite) e um botao "Enviar"
 * O esp32 recebe e printa no Serial.
 * 
 *Fim de codigo.
 */

//Sobre o carrinho...
//porta 23 e 22 é a roda esquerda
//porta 17 e 16  é a roda direita
//porta 19 e 18 é os encoders
//.........................

// Load Wi-Fi library
#include <WiFi.h>

// Credenciais para entrar na rede de internet
const char* ssid = "Raiolux"; //nome da rede
const char* password = "raiolux1020"; //senha da rede

//Webserver será alocado na porta 80
WiFiServer server(80);


void setup() {
  Serial.begin(115200);//Comunicação com o serial do arduino.

  // Printando texto mostrando que voce esta conectando
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //Função que se conencta a rede
  
  while (WiFi.status() != WL_CONNECTED) { //Enquanto ele nao se conectar, esperamos printando um ponto a cada 500 ms
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");//Conta que se conectou e mostra o IP
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());//ip local (somente disponivel no seu wifi)
  server.begin();//Esta função inicia um servidor.
}









void loop(){
  WiFiClient client = server.available();// Server.available procura por clients. Se nao encontrar, retorna false.

  if (client) {// Se ele encontrar, client se torna um objeto diferente de false, ou seja, verdadeiro.
    Serial.println("New Client Connected.");
    
    while (client.connected()) {// Enquanto o client estiver conectado...
      
    client.println("<!DOCTYPE html><html>");//Estou enviando informaçoes do tipo html
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");//Envia html para o client tornando a pagina pelo menos responsiva
    client.println("<link rel=\"icon\" href=\"data:,\">");//Para tirar o Favicon (??? - sinceramente, nao faz muita dif nao kkk)

//CSS text to style
//botoes (posicionamento)
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style></head>");  
    // Web Page Heading
    client.println("<h1>Hello World</h1>");
      if (client.available()) {//Retorna o numero de bytes diponiveis para leitura. Como é diferente de zero, é verdadeiro.

      
      }//fecha o enquanto do cliente conectado
    client.stop();//Encerra a comunicação.
    Serial.println("Client disconnected.");
    Serial.println("");
    }
  }
}
