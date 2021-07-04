/***********************************************************************
*! \file:                   mozilla_iot.c
*  \projekt:                ESP32 Basic Style
*  \created on:             04.06.2021
*  \author:                 R. Gräber
*  \version:                0
*  \history:                0
*  \brief                   handles IOT Function
***********************************************************************/
 
/***********************************************************************
* Includes
***********************************************************************/
#include "mozilla_iot.h"
#include "configuration.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
//https://iot.mozilla.org/gateway/
//https://iot.mozilla.org/wot/
//https://discourse.mozilla.org/t/webthing-arduino-thingproperty-unit/43918/9
//https://discourse.mozilla.org/t/multilevelsensor-and-onoffswitch/55939/3
/***********************************************************************
* Declarations
***********************************************************************/
#ifdef _mozilla_iot_enable_

//ThingActionObject *action_generator(DynamicJsonDocument *);

WebThingAdapter *adapter;

const char *deviceTypes[] = {"MultiLevelSensor", nullptr};
ThingDevice IoTDev("RSSI Sensor", "RSSISensor", deviceTypes);
ThingProperty SignalStrength("id_rssi", "SignalStrength of WLan signal", INTEGER,
                    "LevelProperty");


#endif // _mozilla_iot_enable_
/***********************************************************************
* local Variable
***********************************************************************/
bool mozilla_iot_enable = 0;
uint16_t mozilla_iot_tcp_port = 8080;
/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
*! \fn          uint8_t get_wlan_status()
*  \brief       return wlan connection status
*  \param       none
*  \exception   none
*  \return      return status of wlan
***********************************************************************/
error_type config_mozilla_iot(){

//Disable code if no switch set
#ifdef _mozilla_iot_enable_

    //**********************local variables*******************
    char thingsID[255];
    IoTDev.id = thingsID;
    uint8_t return_code = er_no_error;
    DynamicJsonDocument ConfigJSON(512);
    //*************** read configuration from config ******

    //Try to open SPIFFS
    if (SPIFFS.begin()) {
        log_i("SPIFFS mounting successfully");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            log_i("reading config file");
            File configFile = SPIFFS.open("/config.json");
            if (configFile) {
                log_i("opened config file");
                // Allocate a buffer to store contents of the file.
                deserializeJson(ConfigJSON, configFile);  
                mozilla_iot_enable = ConfigJSON["MozillaIOT_Enablep"];
                mozilla_iot_tcp_port = ConfigJSON["MozillaIOT_Port"];
                Serial.println(mozilla_iot_tcp_port);
                configFile.close();
            }
        }else{
            return_code = er_no_config_file;
        }
    } else {
        log_e("failed to mount FS");
        return_code = er_spiffs_fault;
    }

    sprintf(thingsID,"urn:dev:ops:%s", WiFi.getHostname());
    //Change ThingsID
    
    //Set Name und IP as new Apater
    log_d("Start WebThingAdapter");
    adapter = new WebThingAdapter(WiFi.getHostname(), WiFi.localIP(),mozilla_iot_tcp_port);

    log_d("Config Propertys");
    IoTDev.description = "Basic ESP, sending RSSI";

    SignalStrength.title = "deviceTypes";
    SignalStrength.unit = "dB";
    SignalStrength.maximum = 100;
    SignalStrength.minimum = 0;
    IoTDev.addProperty(&SignalStrength);

    adapter->addDevice(&IoTDev);
    adapter->begin();

    // set initial values
    ThingPropertyValue initialRSSI = {.integer = WiFi.RSSI()};
    SignalStrength.setValue(initialRSSI);
    (void)SignalStrength.changedValueOrNull();


    return return_code;

#endif

}

/***********************************************************************
*! \fn          uint8_t get_wlan_status()
*  \brief       return wlan connection status
*  \param       none
*  \exception   none
*  \return      return status of wlan
***********************************************************************/
void update_iot(){


}