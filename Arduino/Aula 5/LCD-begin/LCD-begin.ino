#include <LiquidCrystal.h>

#define rs 12
#define en 11
#define d4 5
#define d5 4
#define d6 3
#define d7 2
LiquidCrystal zezim(rs, en, d4, d5, d6, d7);    //Configuração biblioteca com datasheet

byte caracter[8] ={B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
byte caracter2[8] ={B10101,B00000,B10101,B01110,B10101,B00100,B10101,B01010};

void setup() {
zezim.begin(16, 2);                   //O tipo do seu LCD 
zezim.clear();                        // Limpa a tela toda
zezim.createChar(0,caracter);
zezim.createChar(1,caracter)
}

void loop() {
zezim.setCursor(0, 0);zezim.print("Essa ferramenta");
zezim.setCursor(0, 1);zezim.write(byte(0));
zezim.setCursor(1, 1);zezim.write(1);
delay(200);
}
