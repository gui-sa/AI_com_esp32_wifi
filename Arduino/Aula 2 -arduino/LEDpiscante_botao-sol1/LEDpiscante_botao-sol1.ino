#define but 2
#define led1 3
#define led2 4
#define led3 5
byte estado = LOW;

void setup() {
  pinMode(but, INPUT); //Defino função do botão como ENTRADA
  pinMode(led1, OUTPUT); //Defino função do botão como SAIDA
  pinMode(led2, OUTPUT); //Defino função do botão como SAIDA
  pinMode(led3, OUTPUT); //Defino função do botão como SAIDA
}

void loop() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  for (int i = 0; i <= 400; i++) {
    if (digitalRead(but) == 1) {
      estado = !estado;
      digitalWrite(led3, estado);
      delay(100); //PREVENINDO BOUNCE
    }
    delay(5);
  }
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  for (int i = 0; i <= 400; i++) {  //2 segundo de delay
    if (digitalRead(but) == 1) {
      estado = !estado;
      digitalWrite(led3, estado);
      delay(100);  
    }
    delay(5);
  }
}
/*
   A ideia é utilizar um botao enquanto o led esta piscando
   o delay para COMPLETAMENTE a leitura: separando em várias "mini" paradinhas e deixando o processador checar é uma solução
*/
