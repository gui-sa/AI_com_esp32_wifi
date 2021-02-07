/*
 * O objetivo deste codigo é obter um grafico que crie uma funcao transferencia de U(pwm) para rotacao (RPM)
 */
//======================Ligacao protoboard:
//Motor Esquerdo
#define N0 21
#define N1 19
#define encoder_esq 36

//Motor Direito
#define N2 5
#define N3 18
#define encoder_dir 23

//PWM config
int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 

/*
 * O motor  de 800 RPM ou 13.33 Hz SEM CARGA...
 * Ou seja, podemo fazer uma funcao que contaos furinhos e protege contra ruidos de malha fechada...
 * 13.33 Hz equivale a um maximo de 0.075 segundos por ciclo. ou 75 ms 
 * Porém.. existe alguns picos de velocidade, e por isso, decidi aumentar para 100 ms
 */


//Logica de controle
int tempo_atras =0;
int tempo_atras2 =0;
int count_dir=0;
int count_esq=0;

float rpm_dir=0;
float rpm_esq=0;

int furos_encoder = 20;

int intensidade_dir=0;
int intensidade_esq=0;

int freq_ref = 200;
float tempo_aquisicao = 200;//ms

float erro_dir;
float erro_esq;

float erro_dir_ant =0 ;
float erro_esq_ant =0 ;

float integral_erro_dir= 0;
float integral_erro_esq= 0;
byte wind_up_esq = true;
byte wind_up_dir = true;


float der_erro_dir = 0;
float der_erro_esq = 0;

float kp = 1.63;
float Ti = 1;
float Td = 0.0;
float N = 1;

void IRAM_ATTR f_int1 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count_dir++;
  
}

void IRAM_ATTR f_int2 (){  // É NECESSARIO ESSE ATRIBUTO IRAM_ATTR  //Esta funcao contabiliza quantas vezes o laser direito deu rising 
  count_esq++;
  
}




void motor_esq(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
  if (dir==0){
    ledcWrite(0, 0); //Utilizando o canal para porta pwm
    ledcWrite(1, intensity); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(0, intensity); //Utilizando o canal para porta pwm
    ledcWrite(1, 0); //Utilizando o canal para porta pwm
  }
}


void motor_dir(int dir, int intensity){    //dir = 0 > frente; dir = 1 =>tras
  if (dir==0){
    ledcWrite(2, 0); //Utilizando o canal para porta pwm
    ledcWrite(3, intensity); //Utilizando o canal para porta pwm
  }
  if (dir==1){
    ledcWrite(2, intensity); //Utilizando o canal para porta pwm
    ledcWrite(3, 0); //Utilizando o canal para porta pwm
  }
}




void setup() {
  ledcSetup(0, freq , res);  // Configurando o canal 0
  ledcSetup(1, freq , res);  // Configurando o canal 1 
  ledcSetup(2, freq , res);  // Configurando o canal 2 
  ledcSetup(3, freq , res);  // Configurando o canal 3 
  ledcAttachPin(N0, 0); //Linkando o canal 0 à porta GPIO N1
  ledcAttachPin(N1, 1); //Linkando o canal 1 à porta GPIO N2
  ledcAttachPin(N2, 2); //Linkando o canal 1 à porta GPIO N2
  ledcAttachPin(N3, 3); //Linkando o canal 1 à porta GPIO N2
  pinMode(encoder_esq,INPUT);
  pinMode(encoder_dir,INPUT);
  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoder_dir),f_int1, FALLING);//Um ciclo do encoder é contado quando ele acaba de acender.
  attachInterrupt(digitalPinToInterrupt(encoder_esq),f_int2, FALLING);//Um ciclo do encoder é contado quando ele acaba de acender.
  tempo_atras =millis();
  tempo_atras2 =millis();  

}








void loop() {
//  if((millis()-tempo_atras2)>1000){
//
//    Ti = Ti/2.0;
//    tempo_atras2 = millis();
//  }
//
//  

  if((millis()-tempo_atras)>tempo_aquisicao){
    detachInterrupt(encoder_dir);
    detachInterrupt(encoder_esq);

    rpm_dir = ((60*1000/furos_encoder)/(millis()-tempo_atras))*count_dir;
    rpm_esq = ((60*1000/furos_encoder)/(millis()-tempo_atras))*count_esq;
    
    tempo_atras = millis();
    
    count_dir = 0;
    count_esq = 0;
    

    erro_dir_ant = erro_dir;
    erro_esq_ant = erro_esq;
            
    erro_dir = freq_ref - rpm_dir;
    erro_esq = freq_ref - rpm_esq;

    der_erro_dir = (erro_dir - erro_dir_ant)/(tempo_aquisicao/1000.0); 
    der_erro_esq = (erro_esq - erro_esq_ant)/(tempo_aquisicao/1000.0);
        
    integral_erro_dir = integral_erro_dir +  erro_dir*(tempo_aquisicao/1000.0);
    integral_erro_esq = integral_erro_esq +  erro_esq*(tempo_aquisicao/1000.0);
    
    intensidade_dir = kp*(erro_dir + integral_erro_dir/Ti + Td*der_erro_dir);
    intensidade_esq = kp*(erro_esq + integral_erro_esq/Ti + Td*der_erro_esq);


    if((intensidade_dir>255)||(intensidade_dir<-100)){
      wind_up_dir = false;//Protecao contra wind-up
    }
    
    if((intensidade_esq>255)|(intensidade_esq<-100)){
      wind_up_esq = false;//Protecao contra wind-up
    }




        
    if(intensidade_dir>255){
      intensidade_dir=255;//limite da planta pwm
    }

    if(intensidade_dir<100){
      intensidade_dir = 100;//limite da planta pwm
    }



    
    if(intensidade_esq>255){
      intensidade_esq=255;//limite da planta pwm
    }
    
    if(intensidade_esq<100){
      intensidade_esq =100;//limite da planta pwm
    }

  

      Serial.print(freq_ref);
      Serial.print(",");
      Serial.print(rpm_esq);
      Serial.print(",");
      Serial.println(rpm_dir);



    motor_dir(1,intensidade_dir);
    motor_esq(1,intensidade_esq);
    

    attachInterrupt(digitalPinToInterrupt(encoder_dir),f_int1, FALLING);//Um ciclo do encoder é contado quando ele acaba de apagar.
    attachInterrupt(digitalPinToInterrupt(encoder_esq),f_int2, FALLING);//Um ciclo do encoder é contado quando ele acaba de apagar.
  }

 
}
