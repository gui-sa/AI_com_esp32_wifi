#include <Wire.h>  //Biblioteca para controle de i2C e/ou SPI

int SDA_esp = 21;//Porta SDA do esp32 node32s
int SCL_esp = 22;//Porta SCL do esp32 node32s
int w_ds = 104;//Endereco de escrita do ds1307
int r_ds = 232;//Endereco de leitura do ds1307
int horario[3];

void readtime(){
  //Posicionando o ponteiro no endereço 0
  char aux, aux2;
  Wire.beginTransmission(w_ds) ;
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(r_ds,3);//pedindo do endereço de leitura, 3 Bytes
  for(int i = 0; i<3;i++){
    while(!Wire.available());
    aux = Wire.read();

    aux2 = (aux & 112)>>4;
    aux = aux & 15;
    aux = aux + aux2;
    
    horario[i] = aux;    
  }
  Wire.endTransmission();

}


void setime(char ho, char mi, char se){
  char aux;//char, pq o write é de 1 Byte
  
  aux = se/10;//Retorna o valor da div
  aux = aux<<4;//À 4 bits à esquerda, esta os decimos
  se = se%10;//Retorna o resto
  se = aux+se;//junta tudo em um codigo só

  aux = mi/10;//Retorna so os decimos
  aux = aux<<4;//À 4 bits à esquerda, esta os decimos
  mi = mi%10;//Retorna o resto ou seja os unitarios
  mi = aux+mi;//junta tudo em um codigo só

  aux = ho/10;//Retorna o valor da div
  aux = aux<<4;//À 4 bits à esquerda, esta os decimos
  ho = ho%10;//Retorna o resto
  ho = aux+ho;//junta tudo em um codigo só

  Wire.beginTransmission(w_ds) ;
  Wire.write(0);
  Wire.write(se);
  Wire.write(mi);
  Wire.write(ho);
  Wire.endTransmission();
}

void setup() {
Serial.begin(115200);
Wire.begin(SDA_esp,SCL_esp);

Serial.print("Iniciando:");

//ENABLE DS1307 CLOCK AND SETTING TO 0 SECONDS
Wire.beginTransmission(w_ds) ;
Wire.write(0);
Wire.endTransmission();

Serial.println("...");

//Primeira configuração de horario
setime(0,0,0);

Serial.println("HORARIO SETADO COM SUCESSO:");
}

void loop() {
  readtime();
  Serial.print(horario[2]);Serial.print(":");Serial.print(horario[1]);Serial.print(":");Serial.println(horario[0]);

  
}
