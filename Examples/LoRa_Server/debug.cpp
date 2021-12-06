#include "debug.h"
#define DEBUG_ENABLE

size_t debug(const char txt[]) {
#ifdef DEBUG_ENABLE
  return Serial.print(txt);
#endif
}

size_t debugln(const char txt[]) {
#ifdef DEBUG_ENABLE
  return Serial.println(txt);
#endif
}


size_t debug(const String txt) {
#ifdef DEBUG_ENABLE
  return Serial.print(txt);
#endif
}

size_t debugln(const String txt) {
#ifdef DEBUG_ENABLE
  return Serial.println(txt);
#endif
}

size_t debug(double txt, int x) {
#ifdef DEBUG_ENABLE
  return Serial.print(txt, x);
#endif
}

size_t debugln(double txt, int x) {
#ifdef DEBUG_ENABLE
  return Serial.println(txt, x);
#endif
}
