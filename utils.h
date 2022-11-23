// You must free the result if result is non-NULL.
#ifndef _UTILS
#define _UTILS
#include "Arduino.h"
String urlencode(String str);
String urldecode(String str);
unsigned char h2int(char c);
#endif
