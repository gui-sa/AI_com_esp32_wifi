/*
 * Não esqueça de erguer as rodas para que o carrinho não sai por ai andando...
 */
/*
 * O objetivo deste código é separar os tasks pelos cores!!
 * Confira: https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/
 *  Aparentemente quando não separamos os tasks, apenas um processador funciona...
 * 
 * A ideia é:
 *  CORE 0: Lida com as ações do robo baseando nos estados das variaveis e nos inputs. O controle e a tomada de decisão esta nesse topico esta nesse core!
 *  CORE 1: Lida com a entrada e construção de graficos. É neste core onde os estados são alterados!! (estados são inputs na tomada de decisão - são inpus mais ´mastigados´.
 * 
 *  RESUMO:
 *    1)EXISTEM VARIAVEIS DE ESTADO (por exemplo, subida, descida, desespero, colisao, falta de força, etc)
 *    2)O core 1 recebe dados e por isso fica mais facil pra ele lidar com a construção de graficos dos dados de entrada da forma mais linda possível.
 *    3)O controle (ações) depende de um input bem construido e por isso o core 0 tem que ser dedicado a constantemente checar os estados para tomar decisão e ainda manter todo o controle do robô de forma eficaz.
 */   


//====================================   Configurações do motor nos pinos  ===============================
int out_21 = 19;
int out_22 = 18;
int out_11 = 17;
int out_12 = 16;

//=====================================   Pinos e variaveis do sensor lm393 e roda furada =================
#define encoder_res 0.314  //rad/furo   a roda tem 20 furos
#define raio_roda 33    //milimetros 
int lm393_direito = 23;
int lm393_esquerdo = 22;
int count_direito = 0;
int count_esquerdo = 0;

//==========================================  Variáveis globais diversas ==================================

#define DEBUG_encoder 0    //Se deseja printar na tela os seriais das variaveis coloque 1, se nao, deixe no 0
#define DEBUG 0
#define MOTOR_LOOP 0    //Se deseja que o motor acelere, depois desacelere nos dois sentidos, digite 1, se nao, deixe no 0
 
float vel_d, vel_e;
int ti_e, ti_d, tf_e, tf_d;
float vel_d_ref, vel_e_ref;

//==========================================  constantes de controle ==================================
#define Kp 2
#define Ki 0.8

//=====================================   Configurações do PWM e dos cores =============================================
int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 
TaskHandle_t action;//Criando task para teste dos motores de forma crescente e decrescente de velocidade


//=============================================================== ISR`s =============================================================
void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count_direito++;
  
}

void IRAM_ATTR f_int2 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising
  count_esquerdo++;
  
}


//================================================== FUNÇÕES DE CONTROLE MECANICO ===================================================

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

//================================================== FUNÇÕES DE LEITURA  ===================================================


//=================================================  MAIN SETUP ============================================================
void setup() {
  if(DEBUG_encoder||DEBUG){Serial.begin(9600);}
  
  
  pinMode(lm393_direito,INPUT);//Setando pinos do lm393 como entrada de dados
  pinMode(lm393_esquerdo,INPUT);//Setando pinos do lm393 como entrada de dados
  

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
  
  attachInterrupt(digitalPinToInterrupt(lm393_direito),f_int1, RISING);//Um ciclo do encoder é contado quando ele acaba de acender.
  attachInterrupt(digitalPinToInterrupt(lm393_esquerdo),f_int2, RISING);//Um ciclo do encoder é contado quando ele acaba de acender.


  xTaskCreatePinnedToCore(                            //Esta função da setup nos tasks.
      action_code, /* Function to implement the task */
      "action_routine", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &action,  /* Task handle. */
      0); /* Core where the task should run */

  }

//=============================================================== CORE 0 LOOP ==============================================
void action_code(void *pvParameters){

  while(1){     //Criando um loop 


    
    if(MOTOR_LOOP){
            for(int i = 0; i<255;i++){
                  motor_direito(0,i);
                  motor_esquerdo(0,i);
                  delay(10);
                }
                      for(int i = 255; i>0;i--){
                        motor_direito(0,i);
                        motor_esquerdo(0,i);
                        delay(10);
                      }
                             delay(2000);
                
                for(int i = 0; i<255;i++){
                  motor_direito(1,i);
                  motor_esquerdo(1,i);
                  delay(10);
                }
                    for(int i = 255; i>0;i--){
                      motor_direito(1,i);
                      motor_esquerdo(1,i);
                      delay(10);
                    }
                             delay(2000);
    }


    if(!MOTOR_LOOP){
        vel_d_ref = 2000; //1 m/s
        vel_e_ref = 2000; //1 m/s       
        float erro_d, erro_e;
        int intensidade_e, intensidade_d;
        /*
         *A ideia é fazer um controle Proporcional apenas, para tentar manter alguma velocidade constante
         */
         erro_d = vel_d_ref - vel_d;
         erro_e = vel_e_ref - vel_e;

         intensidade_d =  Kp*erro_d;
         intensidade_e =  Kp*erro_e;   
         if(intensidade_d>255)
           intensidade_d=255;
         if(intensidade_e>255)
           intensidade_e=255;
         if(intensidade_d<110)
           intensidade_d=110;
         if(intensidade_e<110)
           intensidade_e=110;

           
         motor_direito(0,intensidade_d);
         motor_esquerdo(0,intensidade_e);

         if(DEBUG){Serial.print(vel_e);Serial.print("\t");Serial.print(vel_d);Serial.print("\t");Serial.print(intensidade_e);Serial.print("\t");Serial.println(intensidade_d); }        
    }

   vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}




//=============================================================== CORE 1 LOOP ==============================================


void loop() {   //void loop roda no core 1 por padrao

  
  if(DEBUG_encoder){Serial.print(millis());Serial.print("\t");Serial.println(vel_d);}

    
    if(count_esquerdo>0){
      tf_e = millis();
      vel_e = ((count_esquerdo)*encoder_res)/((tf_e-ti_e)/1000.0);  //velocidade angular dada por  = quantos pinos foram detectados*quantos rads/pino dividos pelo delta tempo 
      vel_e = vel_e*raio_roda; //mm/s
      count_esquerdo = 0;
      ti_e = millis();
    }
    
    if(count_direito>0){
      tf_d = millis();
      vel_d = ((count_direito)*encoder_res)/((tf_d-ti_d)/1000.0);  //velocidade angular dada por  = quantos pinos foram detectados*quantos rads/pino dividos pelo delta tempo 
      vel_d = vel_d*raio_roda;//mm/s
      count_direito = 0;
      ti_d = millis();
    }

    if((millis()-ti_d)>100){
      vel_d=0;
    }
    if((millis()-ti_e)>100){
      vel_e=0;
    }
}
