
//Motores:
#define motor1a 23
#define motor1b 22
#define motor2a 16
#define motor2b 17

//Encoder:
#define encoder1 19
#define encoder2 18
unsigned long count_direito=0;
unsigned long count_esquerdo=0;

//pwm:
int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 



//========================================================================================

void motor_esquerdo(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
  if (dir==0){
    ledcWrite(3, 0); //Utilizando o canal para porta pwm
    ledcWrite(2, intensity); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(3, intensity); //Utilizando o canal para porta pwm
    ledcWrite(2, 0); //Utilizando o canal para porta pwm
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


//========================================================================================
//Interrupts dos encoders

void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count_direito++;
  
}

void IRAM_ATTR f_int2 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising
  count_esquerdo++;
  
}


//========================================================================================



void setup() {


//encoder
pinMode(encoder1,INPUT);
pinMode(encoder2,INPUT);
attachInterrupt(digitalPinToInterrupt(encoder2),f_int1, RISING);//Um ciclo do encoder é contado quando ele acaba de acender.
attachInterrupt(digitalPinToInterrupt(encoder1),f_int2, RISING);//Um ciclo do encoder é contado quando ele acaba de acender.


//Configurando PWM e portas dos motores
ledcSetup(0, freq , res);  // Configurando o canal 0
ledcSetup(1, freq , res);  // Configurando o canal 1 
ledcSetup(2, freq , res);  // Configurando o canal 2
ledcSetup(3, freq , res);  // Configurando o canal 3
ledcAttachPin(motor2a, 0); //Linkando o canal 0 à porta GPIO out_21
ledcAttachPin(motor2b, 1); //Linkando o canal 1 à porta GPIO out_22
ledcAttachPin(motor1a, 2); //Linkando o canal 2 à porta GPIO out_11
ledcAttachPin(motor1b, 3); //Linkando o canal 3 à porta GPIO out_12
//Fim das configuraçoes do pwm e dos motores


}

void loop() {
motor_esquerdo(0,255);
motor_direito(0,255);

}
