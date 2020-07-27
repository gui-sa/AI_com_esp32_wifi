#include <EEPROM.h>
/*
 * Memoria EEPROM: Memória NAO VOLÁTIL
 * No caso do arduino UNO o tamanho total (visualisado no datasheet) de 1KB->1024 Bytes
 *EEPROM.write(endereço de 0->1023,variavel de desejo de 1 Byte)
 *EEPROM.read(endereço de 0->1023)
 */
#define eeprom_comp 1024


void limpar_eeprom(){
  for(int ad = 0; ad < eeprom_comp ; ad++){
    EEPROM.write(ad,0);
  }
}
void EEPROMWriteint(int adress, int valor){
  byte hibyte = highByte(valor);    //pega do 8*->16* bits 
  byte lobyte = lowByte(valor);     //pega do 0*->7* bit
  EEPROM.write(adress,hibyte);      //Grava o Byte mais significativo (mais a esquerda)
  EEPROM.write(adress+1,lobyte);    //Grava o Byte menos significativo (mais a direita)

}
void EEPROMWriteint2(int adress, int valor){
  byte hibyte = valor>>8;    //pega do 8*->16* bits 
  byte lobyte = valor&255;     //pega do 0*->7* bit
  EEPROM.write(adress,hibyte);      //Grava o Byte mais significativo (mais a esquerda)
  EEPROM.write(adress+1,lobyte);    //Grava o Byte menos significativo (mais a direita)

}
int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}
int EEPROMReadInt2(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return ((hiByte<<8)|loByte); 
}


void setup() {
Serial.begin(9600);
//limpar_eeprom();
//EEPROM.write(995,192);
EEPROMWriteint2(0, 9999);
}

void loop() {
if(Serial.available()){
  char leitura = Serial.read();
  if(leitura=='a'){
    leitura = EEPROM.read(995);
    Serial.println((int)leitura);
  }
    if(leitura=='b'){
    int leitura2 = EEPROMReadInt2(0);
    Serial.println((int)leitura2);
  }
}

}
