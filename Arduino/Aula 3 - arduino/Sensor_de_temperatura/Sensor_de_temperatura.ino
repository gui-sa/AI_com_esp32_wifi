//-----------------------------PORTAS-----------------------------------------------------//

int LM35= A0;

//-----------------------------VARIAVEIS---------------------------------------------------//

float temp=0;   //Variável que armazena temperatura

//-----------------------------MAIN PROGRAM------------------------------------------------//
void setup() {
Serial.begin(9600);   //Iniciando comunicação com o computador via serial

}

void loop() {
temp=analogRead(LM35); temp = (temp*5/1023)/0.01;  // Coleta 0-1023 (de 0-5V)-> Converte analog para tensao (5/1023) -> Interpreta tensao recebida com a temperatura cada 0.01 V temos o acrescimo de 1 grau celsiu;
Serial.print("Temperatura: ");  //Printa no Serial.
Serial.println(temp);  //Printa no serial pulando uma linha
delay(1000);    //Aguarda 1 segundo para a proxima medição
}
