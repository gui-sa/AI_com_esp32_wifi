/*
 * O objetivo deste codigo é obter um grafico que crie uma funcao transferencia de U(pwm) para rotacao (RPM)
 */
//Ligacao protoboard:

#define N1 23
#define N2 22
#define lm393 21

int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 

/*
 * O motor  de 800 RPM ou 13.33 Hz SEM CARGA...
 * Ou seja, podemo fazer uma funcao que contaos furinhos e protege contra ruidos de malha fechada...
 * 13.33 Hz equivale a um maximo de 0.075 segundos por ciclo. ou 75 ms 
 * Porém.. existe alguns picos de velocidade, e por isso, decidi aumentar para 100 ms
 */



int tempo_atras =0;
int tempo_teste=0;
int count=0;
float rpm=0;
int furos_encoder = 20;
void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count++;
  
}





void motor(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
  if (dir==0){
    ledcWrite(0, 0); //Utilizando o canal para porta pwm
    ledcWrite(1, intensity); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(0, intensity); //Utilizando o canal para porta pwm
    ledcWrite(1, 0); //Utilizando o canal para porta pwm
  }
}



void setup() {
  ledcSetup(0, freq , res);  // Configurando o canal 0
  ledcSetup(1, freq , res);  // Configurando o canal 1 
  ledcAttachPin(N1, 0); //Linkando o canal 0 à porta GPIO N1
  ledcAttachPin(N2, 1); //Linkando o canal 1 à porta GPIO N2
  
  pinMode(lm393,INPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(lm393),f_int1, FALLING);//Um ciclo do encoder é contado quando ele acaba de acender.
  tempo_atras =millis();
  tempo_teste=millis();
  delay(1000); 
}








void loop() {
  
  while((millis()-tempo_teste)<5000){
    
      motor(0,153);
    
      if((millis()-tempo_atras)>100){
        detachInterrupt(lm393);
        rpm = ((60*1000/furos_encoder)/(millis()-tempo_atras))*count;
        tempo_atras = millis();
        count = 0;
        Serial.println(rpm);
        attachInterrupt(digitalPinToInterrupt(lm393),f_int1, FALLING);//Um ciclo do encoder é contado quando ele acaba de apagar.
      }
  }
  motor(0,0);
}
