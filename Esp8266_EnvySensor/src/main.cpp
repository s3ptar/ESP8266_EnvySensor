#include <Arduino.h>
#include "settings.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include <Thing.h>
#include <WebThingAdapter.h>
#include "configuration.h"
#include "stdint.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
//https://resource.heltec.cn/download/WiFi_Kit_32/WIFI_Kit_32_Schematic_diagram_V2.pdf
//https://iot.mozilla.org/framework/
/***********************************************************************
* Declarations
***********************************************************************/
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");
HardwareSerial SerialCom1(1);
/***********************************************************************
* Global Variable
***********************************************************************/
//Enter your SSID and PASSWORD
const char* ssid = "chilihotdog";
const char* password = "bxJHckMMkGqEPfY3Jf3nZnAn5FtGYwKZSkzVvbzFHNbpUZfv79GXm8afDuNu";

/***********************************************************************
* Constant
***********************************************************************/
time_t now;
char strftime_buf[64];
struct tm timeinfo;
char msgBuffer[1024];

/***********************************************************************
* Local Funtions
***********************************************************************/



/***********************************************************************
*! \fn          void execption_handling()
*  \brief       handle the execption
*  \param       uint32_t error_code - error code
*  \exception   none
*  \return      none
***********************************************************************/


/***********************************************************************
*! \fn          void setup()
*  \brief       Arduino Setup - Routine
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void setup() {
    // put your setup code here, to run once:
    //############ local Variablen #################################

    //start debug on serial
    Serial.begin(115200);
    //enable Debugging
    Serial.setDebugOutput(true);

    //build flags in ini - file
    /*esp_log_level_set("*", ESP_LOG_VERBOSE);
    ESP_LOGD("EXAMPLE", "This doesn't show");

    ESP_LOGE - error (lowest)
    ESP_LOGW - warning
    ESP_LOGI - info
    ESP_LOGD - debug
    ESP_LOGV - verbose (highest)
    */
    DEBUG_ESP_PRINTF("booting");

    //###################################################################################
    //start Configuration Modus
    restore_configuration();
    //###################################################################################
    //###################################################################################
    //connect wlan
    system_delay(2000); 
    connect_wlan();
    system_delay(2000);




}

/***********************************************************************
*! \fn          void loop()
*  \brief       construct host command and send to graphics controller
*  \param       uint32_t HostCommand
*  \exception   none
*  \return      none
***********************************************************************/
void loop() {
  // put your main code here, to run repeatedly:
}