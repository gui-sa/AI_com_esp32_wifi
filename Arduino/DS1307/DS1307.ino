#include <Wire.h>

//_________________________________-Portas do arduino-_____________________________________//

//______________________________-Variaveis e endereços IIC-________________________________//
/*Logica de ESCRITA do IIC:
   Manda o endereçamento geral, do DS1307 para ESCRITA
   Ele envia o "ack" para o arduino
   Envio o endereço do registrador que deseja editar -> aponta o ponteiro para o endereço de memoria
   Ele envia o "ack" para o arduino
   Enviar os dados em formato de 1 Byte(leitura em binário de acordo com o datasheet);
   Ele envia o "ack" para o arduino (Para cada Byte de informação, ele troca de registrador e envia o ack bit de novo)
   Enviar o "Nack" para finalizar comunicação com o DS1307
*/
#define ds1307w 104             //Endereçamento do DS1307 para escrita
#define ds1307r 232             //Endereçamento do DS1307 para leitura
#define reg_seg 0               //Endereçamento do ds1307 que coloca o ponteiro para o registrador de "segundos"
char horario[6];                 // segundos, minutos, horas
//__________________________-Variaveis Globais-_________________________________//

void Ds_init() {            // Deve ser carregada apenas na primeira (caso tenha pilha) ->Disable do bit CH
  Wire.begin();             // Inicia a comunicação Serial IIC DO ARDUINO: pin A4= SDA, pin A5= SCL
  Wire.beginTransmission(ds1307w);          // Endereço do DS1307 + bit W
  Wire.write(reg_seg);                      // Endereço do registrador para colocar ponteiro de escrita em segundos
  Wire.write(0);                            // Manda um Byte 0b00000000 -> Disablando CH e SETANDO para zero segundos
  Wire.endTransmission();                   // Envio do bit "Nack" demonstrando FIM de conecção
}
void Ds_write(int hours, int minutes, int seconds) {
  if (seconds > 59) {seconds = 0;}          //Testa se os valores são convenientes
  if (minutes > 59) {minutes = 0;}          
  if (hours > 23) {hours = 0;}
  char aux; char data=0;                       //Criando variáveis auxiliares                     
  Wire.beginTransmission(ds1307w);          //Envia endereço do DS1307 + bit Write
  Wire.write(reg_seg);
  aux = seconds % 10; seconds = seconds / 10;
  data = (aux | (seconds << 4));
  Wire.write(data);
  aux = minutes % 10; minutes = minutes / 10;
  data = (aux | (minutes << 4));
  Wire.write(data);
  aux = hours % 10; hours = hours / 10;
  data = (aux | (hours << 4));
  Wire.write(data);
  Wire.endTransmission();
}
void Ds_read(char quantidade_byte, char *info){ 
  char aux, dat;
  Wire.beginTransmission(ds1307w);
  Wire.write(reg_seg);
  Wire.endTransmission();
  Wire.requestFrom(ds1307r,quantidade_byte);
  for(int i=0; i<quantidade_byte*2;i=i+2){
    while(!Wire.available());
    dat = Wire.read();
    info[i+1] = (dat & 15)+48;
    dat = dat & 112; info[i]  = (dat>>4)+48;
  }
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Ds_init();
  Serial.println("Iniciando...");
  Ds_write(15,10,00) ;
  Ds_read(3,horario) ;
  Serial.println("Pronto para uso!!");
}

void loop() {
  delay(30);
  Ds_read(3,horario) ;
  Serial.print(horario[4]);Serial.print(horario[5]);Serial.print(":");Serial.print(horario[2]);Serial.print(horario[3]);Serial.print(":");Serial.print(horario[0]);Serial.println(horario[1]);
  delay(220);
}
