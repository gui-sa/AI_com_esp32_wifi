/*A resolução do esp 32 tem 12 bits de ADC (leitura analogica) e 3.3 V*/
/*Todas GPIOs sao pwm. Temos 16 canais disponiveis.*/

//Configurações do motor
int out_21 = 19;
int out_22 = 18;
int out_11 = 17;
int out_12 = 16;


int res = 8;//Numero de bits para resoluçao
int freq = 5000;  //Frequencia do pwm 

void setup() {

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
}

void loop() {

ledcWrite(0, 255); //Utilizando o canal para porta pwm
ledcWrite(1, 0); //Utilizando o canal para porta pwm
ledcWrite(2, 255); //Utilizando o canal para porta pwm
ledcWrite(3, 0); //Utilizando o canal para porta pwm

}
