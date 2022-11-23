#ifndef _CONFIG
#define _CONFIG

#include "secure.h"
/* 
 *  create a scure.h file with your credentials:
 *  
 *  
 #ifndef _SECURE
#define _SECURE
const char *ssid = "yournet";
const char *password = "yourpass";
String GOOGLE_SCRIPT_ID = "your_key";
 #endif
 */

#define PILLID 2
 
#define LED_PIN 4
#define SOILPIN 16
#define DHTPIN  17
#define DHTTYPE DHT11

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

#if NETID ==  POLYHEDRA
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
#define FREQ 3
#endif

#if NETID ==  HOTSPOT
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
#define FREQ 3
#endif

#if NETID ==  BOTANICAL
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */
#define FREQ 60
#endif

#define WIFI_TIMEOUT 30
#endif
