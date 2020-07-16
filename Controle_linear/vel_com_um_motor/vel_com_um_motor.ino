//===================================================  PINOUTS
#define DEBUG 1
#define motor_A1 23
#define motor_A2 22
#define lm393 21

//===================================================  PARAMETROS MECANICOS
float res_lm = 0.314; // rad entre dois furos
float raio = 33.0;  //mm

//===================================================== Variaveis globais
int count = 0;
int count_lm1[] ={0,0,0};
int t1[] = {0,0,0};
int i=0;
char ask = 0;
char stats_vel = 0;
char stats =0;
float vel = 0;

//=====================================   Configurações do PWM e dos cores =============================================
int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 
TaskHandle_t action;//Criando task para teste dos motores de forma crescente e decrescente de velocidade


//=============================================================== ISR`s =============================================================
void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count++;
  i++;
}



void motor_A(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
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
  disableCore0WDT();
  if(DEBUG)
    Serial.begin(9600);
  
  pinMode (lm393,INPUT);

  
  //Configurando PWM e portas dos motores
  ledcSetup(0, freq , res);  // Configurando o canal 0
  ledcSetup(1, freq , res);  // Configurando o canal 1 
  ledcAttachPin(motor_A1, 0); //Linkando o canal 0 à porta GPIO out_21
  ledcAttachPin(motor_A2, 1); //Linkando o canal 1 à porta GPIO out_22
  //Fim das configuraçoes do pwm e dos motores

  attachInterrupt(digitalPinToInterrupt(lm393),f_int1, RISING);//Um ciclo do encoder é contado quando ele acaba de acender.

  
  xTaskCreatePinnedToCore(                            //Esta função da setup nos tasks.
      action_code, /* Function to implement the task */
      "action_routine", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &action,  /* Task handle. */
      0); /* Core where the task should run */
}


void action_code(void *pvParameters){
  while(1){
    float erro, kp , ki , Ie;
    int intensidade;
    float vel_ref = 100; //500 Hz
    kp = 3;kp = 0 ;
      erro = vel_ref-vel; 
      Ie = vel*(t1[2]-t1[1]);
      intensidade = 125 + erro*kp + Ie*ki;
      if (intensidade>255)
        intensidade=255;
      if (intensidade<0)
        intensidade=0;
          
      motor_A(0,intensidade);
  
  
  
  
  }
  
}


void loop() {

  if(Serial.available()){
    ask = Serial.read();
    if(ask=='a')
      Serial.println(count);
    if(ask=='v')
      stats = !stats;    
  }

  if(i >0){
  count_lm1[0] = count_lm1[1];
  t1[0] = t1[1];
  count_lm1[1] = count_lm1[2];
  t1[1] = t1[2];
  count_lm1[2] = count;
  t1[2] = millis();
  i=0;
  }
  
  vel = (count_lm1[2]-count_lm1[1])*res/((millis()-t1[1])/1000.0); //Hz
  

  if(stats)
    Serial.println(vel);
    
}
