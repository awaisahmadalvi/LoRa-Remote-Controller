#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
//#include <inttypes.h>

size_t debug(const char[]);
size_t debugln(const char[]);

size_t debug(const String txt);
size_t debugln(const String txt);

size_t debug(double, int = 2);
size_t debugln(double, int = 2);

#endif
