/*
 * Não esqueça de erguer as rodas para que o carrinho não sai por ai andando...
 */



//Configurações do motor
int out_21 = 19;
int out_22 = 18;
int out_11 = 17;
int out_12 = 16;
//======================

//==========================================================Pinos e variaveis do sensor lm393 e roda furada
float encoder_res = 0.314;  //rad/furo   a roda tem 20 furos
float  raio_roda = 33;    //milimetros 
int lm393_direito = 23;
int lm393_esquerdo = 22;
int count_direito = 0;
int count_esquerdo = 0;
float vel_d, vel_e;
//==========================================================================================================



int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 

//=============================================================== FUNÇOES ==============================================
void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count_direito++;
  
}

void IRAM_ATTR f_int2 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising
  count_esquerdo++;
  
}



void velocity_read(int count_dir,int count_esq,float delta){
  vel_d = ((count_direito-count_dir)*encoder_res)/(delta/1000.0);  //velocidade angular dada por  = quantos pinos foram detectados*quantos rads/pino dividos pelo delta tempo 
  vel_d = vel_d*raio_roda;//mm/s
  vel_e = ((count_esquerdo-count_esq)*encoder_res)/(delta/1000.0);  //velocidade angular dada por  = quantos pinos foram detectados*quantos rads/pino dividos pelo delta tempo 
  vel_e = vel_e*raio_roda; //mm/s
  
}







void motor_esquerdo(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
  if (dir==0){
    ledcWrite(2, 0); //Utilizando o canal para porta pwm
    ledcWrite(3, intensity); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(2, intensity); //Utilizando o canal para porta pwm
    ledcWrite(3, 0); //Utilizando o canal para porta pwm
  }
}


void motor_direito(char dir, int intensity){
  if (dir==0){
    ledcWrite(0, intensity); //Utilizando o canal para porta pwm
    ledcWrite(1, 0); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(0, 0); //Utilizando o canal para porta pwm
    ledcWrite(1, intensity); //Utilizando o canal para porta pwm
  }
}

//=============================================================== MAIN SETUP ==============================================
void setup() {
  
  Serial.begin(9600);
  
  pinMode(lm393_direito,INPUT);
  pinMode(lm393_esquerdo,INPUT);
  

  //Configurando PWM e portas dos motores
  ledcSetup(0, freq , res);  // Configurando o canal 0
  ledcSetup(1, freq , res);  // Configurando o canal 1 
  ledcSetup(2, freq , res);  // Configurando o canal 2
  ledcSetup(3, freq , res);  // Configurando o canal 3
  ledcAttachPin(out_21, 0); //Linkando o canal 0 à porta GPIO out_21
  ledcAttachPin(out_22, 1); //Linkando o canal 1 à porta GPIO out_22
  ledcAttachPin(out_11, 2); //Linkando o canal 2 à porta GPIO out_11
  ledcAttachPin(out_12, 3); //Linkando o canal 3 à porta GPIO out_12
  //Fim das configuraçoes do pwm e dos motores
  
  attachInterrupt(digitalPinToInterrupt(lm393_direito),f_int1, RISING);
  attachInterrupt(digitalPinToInterrupt(lm393_esquerdo),f_int2, RISING);


}


//=============================================================== MAIN LOOP ==============================================

void loop() {
  int c_esq, c_dir;
  for(int i = 0; i<255;i++){
    motor_direito(0,i);
    motor_esquerdo(0,i);
    c_dir=count_direito;
    c_esq=count_esquerdo;
    delay(10);
    velocity_read(c_dir,c_esq,10);
    Serial.println(vel_d);
  }
  
  for(int i = 255; i>0;i--){
    motor_direito(0,i);
    motor_esquerdo(0,i);
    c_dir=count_direito;
    c_esq=count_esquerdo;
    delay(10);
    velocity_read(c_dir,c_esq,10);
    Serial.println(vel_d);
  }
  delay(2000);
  
  for(int i = 0; i<255;i++){
    motor_direito(1,i);
    motor_esquerdo(1,i);
    c_dir=count_direito;
    c_esq=count_esquerdo;
    delay(10);
    velocity_read(c_dir,c_esq,10);
    Serial.println(vel_d);
  }
  
  for(int i = 255; i>0;i--){
    motor_direito(1,i);
    motor_esquerdo(1,i);
    c_dir=count_direito;
    c_esq=count_esquerdo;
    delay(10);
    velocity_read(c_dir,c_esq,10);
    Serial.println(vel_d);
  }
  
  delay(2000);

}
