#include <WiFi.h>

const char* ssid = "nomeDaRede";
const char* password = "senhaDaRede";
WiFiServer server(80); //creates server object 


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
   
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  server.begin();
}

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String response = "";                // make a String to hold incoming data from the client
    String route = "";
    while (client.connected()) {            // loop while the client's connected
      //Quando o cliente realiza uma requisicao, esta parte le o pacote que chega 
      if (client.available()) {             
        Serial.println("Atenção !!!: ");
        while(client.available()){
          char c = client.read();
          response += c;
        }
        int routeIndex =  response.indexOf("GET");
        if(routeIndex>=0){
          routeIndex += 4;
          while(response[routeIndex]!=' '){
            route += response[routeIndex];
            routeIndex++;
          } 
        }
      }
      //Daqui para frente sao rotas e comandos 
      if(route=="/abc"){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        //Envio da pagina HTML 
        client.print("<h1>Parabens! Voce mudou de rota!</h1><p>Clique <a href=\"/\">Aqui</a> para voltar.</p>");
        client.println();
        //Fim do envio da pagina HTML
        //Se não sairmos do loop, ele vai ficar enviando dados sem fim:
        break;
      }

      //Rota padrão! Qualquer coisa digitada vai vir para a pagina Home
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();
      //Envio da pagina HTML 
      client.print("<h1>HOME</h1><p>Clique <a href=\"/abc\">aqui</a> para mudar de rota!</p>");
      client.println();
      //Fim do envio da pagina HTML
      //Se não sairmos do loop, ele vai ficar enviando dados sem fim:
      break;
    }
    // Limpa as variaveis e fecha o cliente.
    response = "";
    route = "";
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
