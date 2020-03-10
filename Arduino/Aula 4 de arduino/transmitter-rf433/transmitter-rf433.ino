#include <VirtualWire.h>

#define data 8
#define w_led 2
#define y_led 3

char dad1 = 'A';              //Codigos enviados
char dad2 = 'B';

void setup() {
  vw_set_tx_pin(data);              //Seta o pino data como TRANSMISSOR DE DADOS
  vw_set_ptt_inverted(true);        //CONFIG PADRAO PARA O SENSOR RF433
  vw_setup(2000);                   //2000 bits por segundo
  pinMode(w_led, INPUT);            //BOTOES
  pinMode(y_led, INPUT);

}

void loop() {
  if (digitalRead(w_led)) {                     //Se apertar esse botao:
    vw_send((uint8_t *)('n'), sizeof(dad1));    //envio dad1(como uint8_t, pois a biblioteca so aceita 1 B nesse formato), no tamanho do uint8t_t
    vw_wait_tx();                               //Espero enviar TOTALMENTE
    delay(200);                                 //Defesa contra o bounce do botao
  }
  if (digitalRead(y_led)) {
    vw_send((uint8_t *)&dad2, sizeof(dad2));
    vw_wait_tx();
    delay(200);
  }
}
