/*A resolução do esp 32 tem 12 bits de ADC (leitura analogica) e 3.3 V*/
/*Todas GPIOs sao pwm. Temos 16 canais disponiveis.*/


int led = 33; //A porta ao qual o led esta inserido
int ledchannel = 0; // O canal PWM ao qual eesta utilizando.
int pot = 36;   //Porta onde esta ligada a leitura analogica
int freq = 5000;  //Frequencia do pwm 
int res = 8;  //Resolução do pwm em bits
int sig;

void setup() {
pinMode(pot,INPUT);
ledcSetup(ledchannel, freq , res);  // Configurando o canal
ledcAttachPin(led, ledchannel); //Linkando o canal à porta GPIO

}

void loop() {


sig = ((analogRead(pot)/4095.0)*255);
ledcWrite(ledchannel, sig); //Utilizando o canal para porta pwm

}
