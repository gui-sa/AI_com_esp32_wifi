#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "SPI.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory
#include "esp_camera.h"


#include "BluetoothSerial.h"

// Config TIMER
long t_stamp = 3600000; // 1 hora
int lifeSpam = 50; // 50 horas e reinicia
String DataPath = "_";


// BLUETOOTH SETTINGS
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


// NETWORK SETTINGS ============================================================
String ssid =  ""; //"Salomao 2.4G";
String password = ""; //"goba918273";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//===== Camera Setting ============================================================
// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

//===== Funcoes ===================================================================
int initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin((const char*)ssid.c_str(), (const char*)password.c_str());
  Serial.print("Connecting to WiFi ..");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    if(count > 10){
      Serial.print("Timed OUT!");
      return 0;
    }
    count++;
  }
  Serial.print("\nPagina de acesso: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
  return 1;
}

void MicroSDInit() { 
   if(!SD_MMC.begin("/sdcard",true)){
    Serial.println("SD Card Mount Failed");
    return;
  }
  pinMode(4,OUTPUT);
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
  Serial.println("SD Card Sucessfully Mounted");
}

void ReadCredential(String path){
  fs::FS &fs = SD_MMC; 
  File file = fs.open(path.c_str(), FILE_READ);
  if(!file){
    Serial.println("Failed to open file in reading mode");
  } 
  else {
    String buff = "";
    while(file.available()){
        char c =  file.read();
        buff += c;
    }
    ssid = buff.substring(0,buff.indexOf("@"));
    password = buff.substring(buff.indexOf("@")+1);
  }
  
  file.close();
}




void OverWriteFile(String path, String buff){
 
  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write((const uint8_t*)buff.c_str(), buff.length()); // payload (image), payload length
    Serial.printf("Saved credentials to path: %s\n", path.c_str());
  }
  
  file.close();
}





void ReadData(String path){
  fs::FS &fs = SD_MMC; 
  File file = fs.open(path.c_str(), FILE_READ);
  if(!file){
    Serial.println("Failed to open file in reading mode");
  } 
  else {
    String buff = "";
    while(file.available()){
        char c =  file.read();
        buff += c;
    }
    DataPath = buff;
    
  }
  
  file.close();
}





void CameraCapture(String path){

  digitalWrite(4,HIGH);
  delay(1000);
   camera_fb_t * fb = NULL;

  // Take Picture with Camera
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }


  
  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
  }
  
  file.close();
  esp_camera_fb_return(fb); 

  digitalWrite(4,LOW);
}


//===== MAIN ===================================================================
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector ... loop esta vazio.
  Serial.begin(115200);
  SPI.begin(13, 12, 14, 4); // SCK, MOSI, MISO, SS


  
//===== Camera Init ======================
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

// Iniciar servicos do MICRO SD  e WIFI(ou bluetooth)  ===================

  MicroSDInit();
  ReadCredential("/credentials"); //ler arquivo de SSID@pass
  int connStatus = initWiFi();
  if(connStatus == 0){
    WiFi.mode( WIFI_MODE_NULL );
    delay(1000);
    SerialBT.begin("MonitorFoto"); //Bluetooth device name
    Serial.println("\nPor favor informe as credenciais por Bluetooth...");
    int timeOut = 0;
    while(true){
        if (Serial.available()) {
          char c = Serial.read();
          ReadCredential("/credentials");
        }
        if (SerialBT.available()) {
          String buff = "";
          while(SerialBT.available()){
            char c = SerialBT.read();
            buff += c;
          }
          OverWriteFile("/credentials",buff);
          for(int i=0; i<WiFi.macAddress().length();i++){
            SerialBT.write((uint8_t)WiFi.macAddress().c_str()[i]);
          }
          delay(1000);
          ESP.restart();
        }
        
        delay(20);
        timeOut++;
        if(timeOut>=300000){
          esp_deep_sleep_start();
        }
    }
  }
 
// Tirando Foto
  CameraCapture("/foto.jpg");


//Servicos de servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.on("/foto", HTTP_GET, [] (AsyncWebServerRequest *request) {
    CameraCapture("/foto.jpg");
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.on("/disconect", HTTP_GET, [] (AsyncWebServerRequest *request) {
    OverWriteFile("/credentials"," @ ");
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.on("/date",
            HTTP_POST,
            [] (AsyncWebServerRequest *request) {},
            NULL,
            [] (AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
              String buff = "";
              for(size_t i=0; i < len; i++){
                buff += (char)data[i];
              }
              OverWriteFile("/data",buff);
              request->send(200);
             });

  server.serveStatic("/", SD_MMC, "/");

  server.begin();

}

void loop() {
  int t_now = millis();
  while((millis()-t_now)<t_stamp);
  ReadData("/data");
  CameraCapture("/"+DataPath+"L"+lifeSpam+".jpg");
  lifeSpam--;
  if(lifeSpam<=0){
    ESP.restart();
  }
}
