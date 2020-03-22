#include <VirtualWire.h>

#define data 8
#define yellow 9
#define white 10

uint8_t receive[sizeof(char)];      //Criando uma variavel de 8 bits com o tamanho de um char
uint8_t tamanho = sizeof(char);    //tamanho do char

char received;                    //char para receber os dados da uint8_t
byte w = LOW;                       //Variaveis booleanas para controle do estado do led
byte y = LOW;                       //Variaveis booleanas para controle do estado do led

void setup() {
pinMode(yellow,OUTPUT);
pinMode(white,OUTPUT);
vw_set_rx_pin(data);              //Setando a porta RECEPTORA DO RF433
vw_setup(2000);              //2000 bits por segundos
vw_rx_start();                //STARTANDO comunicaçao ASK
}

void loop() {
  if(vw_have_message()){                //Tem mensagem? >se sim:
    vw_get_message(receive,&tamanho);       //Coloque a mensagem no vetor uint8_t receive de tamanho char (nesse caso) no vetor reservado (receive)
    memcpy(&received,receive,tamanho);      //INTERPRETE o vetor receive que é uint8_t de tamanho char (nesse caso) como uma variavel char
    delay(5);                               
    if(received=='A'){          //Se recebeu 'A'
      w =!w;                      //Inverte variavel booleana de estado dos leds
    }
    if(received=='B'){
      y=!y;
    }
    received = '\0';               //LIMPA A VARIAVEL DE RECEPÇAO DE DADOS "RECEIVED", no caso considerei '\0' como o char vazio
  }
  digitalWrite(white,w);      //Escreve o estado da variavel booleana no led
  digitalWrite(yellow,y);
}
