/***********************************************************************
*! \file:                   settings.h
*  \projekt:                globale Variablen
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   abgeleitet von tasmota sonoff
***********************************************************************/
 
#ifndef _settings_H_
#define _settings_H_
 
/***********************************************************************
* Includes
**********************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "config.h"
/***********************************************************************
* Informations
**********************************************************************/
//https://download.bastelgarage.ch/Datasheet/ESP8266_Heltec_Wifi_Kit_Schema.png
 
/***********************************************************************
* Declarations
**********************************************************************/



/***********************************************************************
* Global Variable
**********************************************************************/

/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/


#define config_file_full_path "/config.json"
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif
#ifndef DeviceClassID
    #define DeviceClassID "ESP"
#else
    #if DeviceClassID == HELTEC
        #define DeviceName "Heltec"
        #define _Heltec_Board_
    #else
        #define DeviceName "ESP8266"
        #define _ESP_Board_
    #endif
#endif

#endif /* _settings_H_ */
