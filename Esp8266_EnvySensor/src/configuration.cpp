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
#include "WiFi.h"
#include "settings.h"
#include "error_codes.h"
#include <FS.h>
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
    if (SPIFFS.begin()) {
        DEBUG_ESP_PRINTF("SPIFFS mounting successfully");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            DEBUG_ESP_PRINTF("reading config file");
            File configFile = SPIFFS.open("/config.json");
            if (configFile) {
                DEBUG_ESP_PRINTF("opened config file");
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
        DEBUG_ESP_PRINTF("failed to mount FS");
        return_code = er_spiffs_fault;
    }
    return return_code ;
    

}

/***********************************************************************
*! \fn          bool get_bool_parameter(uint8_t parameter)
*  \brief       return requestet boolean parameter
*  \param       uint8_t parameter
*  \exception   none
*  \return      boolean
***********************************************************************/


/***********************************************************************
*! \fn          char* get_bool_parameter(uint8_t parameter)
*  \brief       return requestet boolean parameter
*  \param       uint8_t parameter
*  \exception   none
*  \return      char pointer
***********************************************************************/
