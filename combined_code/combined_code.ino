#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <cstdlib>

#include "SensorReading.h"
#include "infoProcessing.h"

// Define pins for each flex sensor (fingers are ordered left to right order on left hand)
#define PINKY 13 
#define RING 12
#define MIDDLE 14 
#define INDEX 27 
#define THUMB 26  

#define MPU_SDA 23
#define MPU_SCL 22

//Define other handy macro constants
#define delayTime 1000
#define POLL_WAIT_TIME 200          // in ms

//Global variables for use, some of these are extern'd by other files.
extern const int sensorPins[5] = {PINKY, RING, MIDDLE, INDEX, THUMB};
const bool learning_mode = true;
const char learning_letter = 'A';
bool calibrating = true;
long calibratingTime = 0;
Adafruit_MPU6050 mpu;
bool zMotion = false;
bool jMotion = false;

void setup() {
  Serial.begin(115200);
  while(!Serial) {delay(10);}
  Serial.println("Serial connected!");

  //Initialize MPU and set proper pins for SDA and SCL to prevent hanging.
  
  if(!learning_mode) {
    Wire.begin(MPU_SDA, MPU_SCL);
    if(!mpu.begin()) { 
      Serial.println("Failed to find MPU6050 chip");
      while(true) delay(10);
    }
    Serial.println("MPU6050 Found!");

    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    mpu.setMotionDetectionThreshold(1);
    mpu.setMotionDetectionDuration(20);
    mpu.setInterruptPinLatch(true);  // Keep it latched
    mpu.setInterruptPinPolarity(true);
    mpu.setMotionInterrupt(true);
  }
  else {Serial.println("MPU initialization skipped due to learning mode!");}

  for(int s = 0; s < 5; s++) {pinMode(sensorPins[s], INPUT);}
  Serial.println("Settings initialized!");
}
  

void loop() {

  if (!learning_mode && mpu.getMotionInterruptStatus()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Print motion data
    Serial.print("Motion Detected! AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(", AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(", AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(", GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(", GyroZ:");
    Serial.println(g.gyro.z);

    if(std::abs(a.acceleration.x) > 10  || std::abs(a.acceleration.z) > 10) { zMotion = true; jMotion = false;}
    else { jMotion = true; zMotion = false;}
    jMotion = true;
  }
  else {Serial.println("No motion!");}

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
