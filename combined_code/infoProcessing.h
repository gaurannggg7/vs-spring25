#include <Arduino.h>
#include <cfloat>
#include "SensorReading.h"

#ifndef INFO_PROCESSING_H
#define INFO_PROCESSING_H
char produceChar(SensorReading &reading);
char* produceWord(char appendedChar);

#endif