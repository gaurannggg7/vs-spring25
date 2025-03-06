#include <Arduino.h>
#include <algorithm>
#include <cmath>
#include <Vector>
#include <queue>

#ifndef SENSOR_READING_H
#define SENSOR_READING_H

class SensorReading {
public:
  int readingVals[5];

  static SensorReading* getReading();
  static void calibrationSetup(SensorReading &reading);
  static void calibrateReading(SensorReading &reading);

  float distToVector(int values[5]);
  bool isValid();
  void print();
  bool stabilize(char &output);
private:
  void copy(const SensorReading &other);
  void operator-=(const SensorReading &other);
  
  static void updateMinMaxReadings(SensorReading &reading);
  // bool const operator <( const SensorReading &other);
  // bool const operator ==(const SensorReading &other);
  // constexpr bool isInvalid();
  // void print();
};
#endif