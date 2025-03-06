#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "SensorReading.h"
#include "infoProcessing.h"

// Define pins for each flex sensor (fingers are ordered left to right order on left hand)
#define PINKY 13 
#define RING 27 
#define MIDDLE 33 
#define INDEX 35 
#define THUMB 34  

//Define other handy macro constants
#define delayTime 1000
#define POLL_WAIT_TIME 200          // in ms

//Global variables for use, some of these are extern'd by other files.
extern const int sensorPins[5] = {PINKY, RING, MIDDLE, INDEX, THUMB};
bool calibrating = true;
long calibratingTime = 0;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while(!Serial) {delay(10);}

  // if(!mpu.begin()) { 
  //   Serial.println("Failed to find MPU6050 chip");
  //   while(true) delay(10);
  // }
  // Serial.println("MPU6050 Found!");

  // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  // mpu.setMotionDetectionThreshold(1);
  // mpu.setMotionDetectionDuration(20);
  // mpu.setInterruptPinLatch(true);  // Keep it latched
  // mpu.setInterruptPinPolarity(true);
  // mpu.setMotionInterrupt(true);

  for(int s = 0; s < 5; s++) {pinMode(sensorPins[s], INPUT);}
  Serial.println("Settings initialized!");
}

void loop() {
  SensorReading &reading = *(SensorReading::getReading());
  if (calibrating) {
    SensorReading::calibrationSetup(reading);
    delay(POLL_WAIT_TIME);
    calibratingTime += POLL_WAIT_TIME;
    return;
  }
  SensorReading::calibrateReading(reading);

  //SensorReading::stabilizeReading(reading);
  char curChar;
  if(reading.stabilize(curChar)) {
    Serial.printf("Reading was stable! Now registering character %c\n", curChar);
    char* producedWord = produceWord(curChar);
    if(producedWord != nullptr) { Serial.printf("Word formed: %s\n\n", producedWord);}
  }
  delay(delayTime);
}
