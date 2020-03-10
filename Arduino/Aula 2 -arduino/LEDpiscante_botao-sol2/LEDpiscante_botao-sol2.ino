#define but 2
#define led1 3
#define led2 4
#define led3 5

byte estado = LOW;

int comp = 0;
int comp_new = 0;

void setup() {
  pinMode(but, INPUT); //Defino função do botão como ENTRADA
  pinMode(led1, OUTPUT); //Defino função do botão como SAIDA
  pinMode(led2, OUTPUT); //Defino função do botão como SAIDA
  pinMode(led3, OUTPUT); //Defino função do botão como SAIDA
  comp = millis();
}
void loop() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  while ((comp_new - comp) <= 2000) {
    if (digitalRead(but) == 1) {
      estado = !estado;
      digitalWrite(led3, estado);
      delay(100);
    }
    comp_new = millis();
  }
  comp = comp_new;
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  while ((comp_new - comp) <= 2000) {
    if (digitalRead(but) == 1) {
      estado = !estado;
      digitalWrite(led3, estado);
      delay(100);
    }
    comp_new = millis();
  }
  comp = comp_new;
}
/*
   A ideia é utilizar um botao enquanto o led esta piscando
   o delay para COMPLETAMENTE a leitura: millis nao para o processador, o que acontece é apenas uma COMPARAÇÃO. Ele conta quantos milisecundos passo desde ligado.
*/
/*vantagens:
 * processamento TOTALMENTE LIVRE dentro dos laços de while
 * maior PRECISÃO nos delays(comparado à primeira solução)
 */
