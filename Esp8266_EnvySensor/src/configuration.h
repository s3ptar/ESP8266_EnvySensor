/***********************************************************************
*! \file:                   configuration.h
*  \projekt:                FT800_ESP
*  \created on:             25.07.2020
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   Definitionsfile for global variables
***********************************************************************/
 
#ifndef _configuration_h_
#define _configuration_h_
 
/***********************************************************************
* Includes
**********************************************************************/
#include "stdint.h"
#include "FS.h"
#include "SPI.h"
#include "error_codes.h"
#include <Arduino.h>
#include "settings.h"
#include "WiFi.h"
#include <ESPAsyncWebServer.h>
#ifdef _mozilla_iot_enable_
    #define ARDUINOJSON_USE_LONG_LONG 1
    #include <Thing.h>
    #include <WebThingAdapter.h>
#endif
#include <ArduinoJson.h>
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
 #define glb_device_name_length  64
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
error_type restore_configuration();
error_type connect_wlan();
error_type config_mozilla_iot();
void update_iot();

#endif //_filehandling_h_