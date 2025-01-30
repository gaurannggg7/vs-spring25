#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Define pins for each flex sensor
#define FLEX_SENSOR_INDEX 25  // Pin for index finger sensor
#define FLEX_SENSOR_MIDDLE 26 // Pin for middle finger sensor
#define FLEX_SENSOR_RING 27   // Pin for ring finger sensor
#define FLEX_SENSOR_PINKY 14  // Pin for pinky finger sensor
#define FLEX_SENSOR_THUMB 33  // Pin for thumb sensor


Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for serial connection

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  // Setup motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  // Pin setup for flex sensors
  pinMode(FLEX_SENSOR_INDEX, INPUT);
  pinMode(FLEX_SENSOR_MIDDLE, INPUT);
  pinMode(FLEX_SENSOR_RING, INPUT);
  pinMode(FLEX_SENSOR_PINKY, INPUT);
  pinMode(FLEX_SENSOR_THUMB, INPUT);
}

void loop() {
  // Read flex sensor values
  int sensorValues[5];
  sensorValues[0] = analogRead(FLEX_SENSOR_INDEX);
  sensorValues[1] = analogRead(FLEX_SENSOR_MIDDLE);
  sensorValues[2] = analogRead(FLEX_SENSOR_RING);
  sensorValues[3] = analogRead(FLEX_SENSOR_PINKY);
  sensorValues[4] = analogRead(FLEX_SENSOR_THUMB);

  // Detect ASL letter
  char detectedLetter = detectASLLetter(sensorValues);

  // Print detected ASL letter
  if (detectedLetter != '?') {
    Serial.print("Detected ASL Letter: ");
    Serial.println(detectedLetter);
  } else {
    Serial.println("No ASL match found.");
  }

  // Check for motion
  if (mpu.getMotionInterruptStatus()) {
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
  }

  // Short delay for stability
  delay(200);
}

// Function to detect ASL letter by calling specific letter methods
char detectASLLetter(int values[]) {
  if (isLetterA(values)) return 'A';
  if (isLetterB(values)) return 'B';
  if (isLetterC(values)) return 'C';
  if (isLetterD(values)) return 'D';
  if (isLetterE(values)) return 'E';
  // Add other letters as required...
  return '?';
}

// ASL Letter Detection Methods
bool isLetterA(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] < 80);
}

bool isLetterB(int values[]) {
  return (values[0] < 50 && values[1] < 50 && values[2] < 50 && values[3] < 50 && values[4] > 10);
}

bool isLetterC(int values[]) {
  return (values[0] >= 60 && values[1] >= 60 && values[2] >= 60 && values[3] >= 60 && values[4] >= 40);
}

bool isLetterD(int values[]) {
  return (values[0] < 50 && values[1] > 50 && values[2] > 100 && values[3] > 100 && values[4] > 20);
}

bool isLetterE(int values[]) {
  return (values[0] > 90 && values[1] > 90 && values[2] > 90 && values[3] > 90 && values[4] > 40);
}

// Add other letter detection methods as required...
