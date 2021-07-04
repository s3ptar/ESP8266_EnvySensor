/***********************************************************************
*! \file:                   configuration.c
*  \projekt:                ESP32 Basic Style
*  \created on:             26.07.2020
*  \author:                 R. Gräber
*  \version:                0
*  \history:                0
*  \brief                   handles dictionary
***********************************************************************/
 
/***********************************************************************
* Includes
***********************************************************************/
#include "configuration.h"
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include "WiFi.h"
#endif
#include "settings.h"
#include "error_codes.h"
#include "LittleFS.h"
#include "string.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Declarations
***********************************************************************/
struct wlan_properties_tags{
   uint8_t wlan_enabled : 1;
   uint8_t wlan_ap_modus : 1;
   uint8_t wlan_OTA : 1;
   uint8_t wlan_status;
   char ssid[32];
   char passphrase[64];
   const char*  ptr_ssid = &ssid[0];
   const char*  ptr_passphrase = &passphrase[0];
};


struct spiffs_flags_tag{
   uint8_t spiff_mounted : 1;
   uint8_t spiff_config_file_found : 1;
};
/***********************************************************************
* local Variable
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/

IPAddress ip;
uint8_t glb_MAC_address[6];
char glb_device_name[glb_device_name_length];
wlan_properties_tags wlan_properties;

/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
*! \fn          void restore_config()
*  \brief       read configuration from SSPIFs, config.json files
*  \param       none
*  \exception   none
*  \return      error_type return error codes
***********************************************************************/
error_type restore_configuration(){

    //****************  localFunction Variables ***********************
    uint8_t return_code = er_no_error;
    DynamicJsonDocument ConfigJSON(512);

    //set dynamic (Chip propertys)
    WiFi.macAddress(glb_MAC_address);
    //set default name
    sprintf(glb_device_name, "%s%02x%02x%02x", DeviceName,glb_MAC_address[3],glb_MAC_address[4],glb_MAC_address[5]);

    //Try to open SPIFFS
    if (LittleFS.begin()) {
        system_log("SPIFFS mounting successfully", dbg_info);
        if (LittleFS.exists("/config.json")) {
            //file exists, reading and loading
            system_log("reading config file", dbg_info);
            File configFile = LittleFS.open(F("/config.json"),  "r");
            if (configFile) {
                system_log("opened config file", dbg_info);
                // Allocate a buffer to store contents of the file.
                deserializeJson(ConfigJSON, configFile);  
                strlcpy(wlan_properties.ssid,        // <- destination
                    ConfigJSON["wlan_data"]["ssid"], // <- source
                    sizeof(wlan_properties.ssid)     // <- destination's capacity
                );  
                strlcpy(wlan_properties.passphrase,        // <- destination
                    ConfigJSON["wlan_data"]["pass"], // <- source
                    sizeof(wlan_properties.passphrase)     // <- destination's capacity
                );
                wlan_properties.wlan_enabled = ConfigJSON["wlan_enable"];
                wlan_properties.wlan_OTA = ConfigJSON["wlan_ota"];
                wlan_properties.wlan_ap_modus = ConfigJSON["wlan_ap"];
                configFile.close();
            }
        }else{
            return_code = er_no_config_file;
        }
    } else {
        system_log("failed to mount FS", dbg_info);
        return_code = er_spiffs_fault;
    }
    return return_code ;
    

}

/***********************************************************************
*! \fn          void system_delay(uint32_t ms_delay)
*  \brief       wait ms
*  \param       uint32_t ms_delay
*  \exception   none
*  \return      none
***********************************************************************/
void system_delay(uint32_t ms_delay){

#if defined(ESP8266)
    delayMicroseconds(ms_delay);
#elif defined(ESP32)
    vTaskDelay(ms_delay);
#endif

}

/***********************************************************************
*! \fn          void system_log(const char *msg, uint8_t InformationClass)
*  \brief       write information to debug output und storage
*  \param       const char *msg - message
*  \param       uint8_t InformationClass - eg information or verbose
*  \exception   none
*  \return      none
***********************************************************************/
void system_log(const char *msg, uint8_t InformationClass){

#if defined(ESP8266)
    
    DEBUG_ESP_PRINTF(msg);

#elif defined(ESP32)
    switch(InformationClass){

        case dbg_error : {
            
            log_e(msg):
            break;
        }

        case dbg_warning: {
            
            log_w(msg):
            break;
        }

        case dbg_info : {

            log_i(msg):
            break;
        }

        case dbg_debug : {

            break;
        }

        case dbg_verbose : {

            log_v(msg):
            break;
        }
        default:{

        }


    }
#endif

}

/***********************************************************************
*! \fn          error_type
*  \brief       return requestet boolean parameter
*  \param       uint8_t parameter
*  \exception   none
*  \return      error_type return error codes
***********************************************************************/
error_type connect_wlan(){

    error_type return_code = er_wlan_disable;
    uint8_t wlan_connection_fail_cnt = wlan_reconnects;

    //WiFi.disconnect(true,true);
    if(wlan_properties.wlan_enabled){
        system_log("WLan On", dbg_verbose);
        //+++++++++++++ check modus 0 = WLan Client, 1 = AccessPoint Modus Modus ++++++
        if(wlan_properties.wlan_ap_modus){
            //AP Modus
            #if defined(ESP8266)
                WiFi.mode(WIFI_AP);
            #elif defined(ESP32)
                WiFi.mode(WIFI_MODE_AP);
            #endif
            return_code = er_wlan_ap_mode;
            system_log("WLan AP Modus", dbg_verbose);
            WiFi.softAP(wlan_properties.ssid, wlan_properties.passphrase);
        }else{
            //Client Modus
            return_code = er_wlan_client_mode;
            #if defined(ESP8266)
                WiFi.mode(WIFI_STA);
            #elif defined(ESP32)
                WiFi.mode(WIFI_MODE_STA);
            #endif
            system_log("WLan Station Modus", dbg_verbose);
            WiFi.setHostname(glb_device_name);
            system_log( glb_device_name, dbg_info);
            WiFi.begin(wlan_properties.ssid, wlan_properties.passphrase); 
            system_log("wait for conntect", dbg_verbose);
            system_delay(2000); 
            while (!WiFi.isConnected()) {
                system_delay(2000);
                system_log("Connection Trys %d", wlan_connection_fail_cnt);
                wlan_connection_fail_cnt--;
                if(!wlan_connection_fail_cnt){
                    //Switch to default WLAN AP Modus if no connect to SSID
                    return_code = er_wlan_default_ap;
                    #if defined(ESP8266)
                        WiFi.mode(WIFI_AP);
                    #elif defined(ESP32)
                        WiFi.mode(WIFI_MODE_AP);
                    #endif
                    system_log("WLan Default AP Modus", dbg_verbose);
                    WiFi.softAP(glb_device_name, "1234567890");
                    //ESP.restart();
                }
            }
            system_log("conntecd!", dbg_verbose);
            ip = WiFi.localIP();

        }
        Serial.println(WiFi.localIP());
    }
    return return_code ;
}