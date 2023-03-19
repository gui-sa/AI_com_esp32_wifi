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

// NETWORK SETTINGS ============================================================
const char* ssid = "Salomao 2.4G";
const char* password = "goba918273";

// MicroSD EEPROM Setting 
#define EEPROM_SIZE 1

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//===== Funcoes ===================================================================
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("\nPagina de acesso: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void MicroSDInit() { 
   if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
  Serial.println("SD Card Sucessfully Mounted");
}

//===== MAIN ===================================================================
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector ... loop esta vazio.
  Serial.begin(115200);
  initWiFi();
  MicroSDInit();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD_MMC, "/index.html", "text/html");
  });

  server.serveStatic("/", SD_MMC, "/");

  server.begin();
}

void loop() {
  
}
