#include "SensorReading.h"
#include "infoProcessing.h"

#define MAX_QUEUE_LENGTH 5
#define STABILITY_CUTOFF 200
#define MAX_CALIBRATING_TIME 10000 // in ms
#define MAX_CALIBRATED_VALUE 100

//Handy global variables, some have been extern'd from other files.
extern const int sensorPins[5];
extern bool calibrating;
extern long calibratingTime;
extern SensorReading curReading;

SensorReading _min;
SensorReading _max;
SensorReading dist; // max - min (calculate after calibrations)

std::queue<SensorReading> history;
char detected = -1;

void SensorReading::operator-=(const SensorReading &other) {
  for(int f = 0; f < 5; f++) { this->readingVals[f] -= other.readingVals[f];}
}

SensorReading* SensorReading::getReading() {
  SensorReading* reading = new SensorReading();
  for(int f = 0; f < 5; f++) {
    reading->readingVals[f] = analogRead(sensorPins[f]);
  }
  return reading;
}

void SensorReading::calibrationSetup(SensorReading &reading) {
  //Keep updating to keep track of finger val ranges
  if(calibratingTime == 0) { _min.copy(reading); _max.copy(reading);} 
  else { updateMinMaxReadings(reading);}

  Serial.println("Max:");
  _max.print();
  Serial.println("Min:");
  _min.print();

  if(calibratingTime > MAX_CALIBRATING_TIME) {
    calibrating = false;
    dist.copy(_max);
    dist -= _min;
    return;
  }
}

void SensorReading::calibrateReading(SensorReading &reading) {
  reading -= _min;
  for(int f = 0; f < 5; f++){
    reading.readingVals[f] = reading.readingVals[f] * MAX_CALIBRATED_VALUE / max(dist.readingVals[f], 1);
  }
}

void SensorReading::updateMinMaxReadings(SensorReading &reading) {
  for(int f = 0; f < 5; f++) {
    _max.readingVals[f] = std::max(_max.readingVals[f], reading.readingVals[f]);
    _min.readingVals[f] = std::min(_min.readingVals[f], reading.readingVals[f]);
  }
}

bool SensorReading::isValid() {
  return true;
  for(int f = 0; f < 5; f++) {
    int fVal = this->readingVals[f];
    if(fVal < 0 || fVal > 1023) { return false;}
  }
  return true;
}

float SensorReading::distToVector(int values[5]) {
  float distance = 0.0f;
  for(int f = 0; f < 5; f++){
    float leg = this->readingVals[f] - values[f];
    leg *= leg;
    distance += leg;
  }
  return sqrtf(distance);
}

void SensorReading::copy(const SensorReading &other) {
  for(int f = 0; f < 5; f++) {this->readingVals[f] = other.readingVals[f];}
}

// void SensorReading::stabilizeReading(SensorReading &reading) {
//   history.push_back(reading);
//   while(history.size() > MAX_QUEUE_LENGTH) {
//     history.erase(history.begin());
//   }

//   if (history.size() < MAX_QUEUE_LENGTH) Serial.println("Calibrating...");
//   if (history.size() == MAX_QUEUE_LENGTH && history.front().distToVector(history.back().readingVals) < STABILITY_CUTOFF) {
//     char temp = produceChar(reading);

//     if (temp <= 0) {
//       Serial.print("Error ");
//       Serial.println(static_cast<int>(temp));
//       // reading.print();
//     }

//     // Serial.println(static_cast<int>(temp));

//     // Serial.println("Reading: " + temp);

//     if (temp != detected) {
//       detected = temp;
//       Serial.println(detected);
//     }
//   } else if (detected != 0) {
//     detected = 0;
//   }
// }

bool SensorReading::stabilize(char &output) {
  output = produceChar(*this);
  if(output != detected) { 
    detected = output;
    history = {}; 
    return false;
  } //Flush history if our reading doesn't match up with previous readings
  else if(output == -1) { detected = -1; return false;}

  history.push(*this);
  if(history.size() == MAX_QUEUE_LENGTH) { output = detected; history = {}; return true;} //Flush history to allow reading of new character, and we could use this for word formation.

  return false;
}

void SensorReading::print() {
  for(int f = 0; f < 5; f++) {
    Serial.print(readingVals[f]);
    Serial.print(" ");
  }
  Serial.println();
}

