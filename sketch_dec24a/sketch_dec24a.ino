// Define pins for each flex sensor
#define FLEX_SENSOR_INDEX 25  // Pin for index finger sensor
#define FLEX_SENSOR_MIDDLE 26 // Pin for middle finger sensor
#define FLEX_SENSOR_RING 27   // Pin for ring finger sensor
#define FLEX_SENSOR_PINKY 14  // Pin for pinky finger sensor
#define FLEX_SENSOR_THUMB 33  // Pin for thumb sensor

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read sensor values
  int sensorValues[5];
  sensorValues[0] = analogRead(FLEX_SENSOR_INDEX);
  sensorValues[1] = analogRead(FLEX_SENSOR_MIDDLE);
  sensorValues[2] = analogRead(FLEX_SENSOR_RING);
  sensorValues[3] = analogRead(FLEX_SENSOR_PINKY);
  sensorValues[4] = analogRead(FLEX_SENSOR_THUMB);

  // Detect ASL letter
  char detectedLetter = detectASLLetter(sensorValues);

  // Print results
  if (detectedLetter != '?') {
    Serial.print("Detected Letter: ");
    Serial.println(detectedLetter);
  } else {
    Serial.println("No match found.");
  }

  delay(200); // Short delay for stability

  Serial.print(sensorValues[4]);
  Serial.print(", ");
  Serial.print(sensorValues[0]);
  Serial.print(", ");
  Serial.print(sensorValues[1]);
  Serial.print(", ");
  Serial.print(sensorValues[2]);
  Serial.print(", ");
  Serial.println(sensorValues[3]);
}



// Function to detect ASL letter by calling specific letter methods
char detectASLLetter(int values[]) {
  if (isLetterA(values)) return 'A';
  if (isLetterB(values)) return 'B';
  if (isLetterC(values)) return 'C';
  if (isLetterD(values)) return 'D';
  if (isLetterE(values)) return 'E';
  if (isLetterF(values)) return 'F';
  if (isLetterG(values)) return 'G';
  if (isLetterH(values)) return 'H';
  if (isLetterI(values)) return 'I';
  if (isLetterK(values)) return 'K';
  if (isLetterL(values)) return 'L';
  if (isLetterM(values)) return 'M';
  if (isLetterN(values)) return 'N';
  if (isLetterO(values)) return 'O';
  if (isLetterP(values)) return 'P';
  if (isLetterQ(values)) return 'Q';
  if (isLetterR(values)) return 'R';
  if (isLetterS(values)) return 'S';
  if (isLetterT(values)) return 'T';
  if (isLetterU(values)) return 'U';
  if (isLetterV(values)) return 'V';
  if (isLetterW(values)) return 'W';
  if (isLetterX(values)) return 'X';
  if (isLetterY(values)) return 'Y';

  return '?'; // No match found


}

// ASL Letter Detection Methods

bool isLetterA(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] < 80);
}

bool isLetterB(int values[]) {
  return (values[0] < 50 && values[1] < 50 && values[2] < 50 && values[3] < 50 && values[4] > 10);
}

bool isLetterC(int values[]) {
  return (values[0] >=60 && values[1] >= 60 && values[2] >= 60 && values[3] >= 60 && values[4] >= 40);
}

bool isLetterD(int values[]) {
  return (values[0] < 50 && values[1] > 50 && values[2] > 100 && values[3] > 100 && values[4] > 20);
}

bool isLetterE(int values[]) {
  return (values[0] > 90 && values[1] > 90 && values[2] > 90 && values[3] > 90 && values[4] > 40);
}

bool isLetterF(int values[]) {
  return (values[0] < 50 && values[1] > 90 && values[2] > 90 && values[3] > 90 && values[4] >50);
}

bool isLetterG(int values[]) {
  return (values[0] < 40 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] < 20);
}

bool isLetterH(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterI(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] < 40 && values[4] > 40);
}

bool isLetterK(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] < 20);
}

bool isLetterL(int values[]) {
  return (values[0] < 40 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] < 40);
}

bool isLetterM(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] < 90 && values[3] < 90 && values[4] > 40);
}

bool isLetterN(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] < 90 && values[4] > 40);
}

bool isLetterO(int values[]) {
  return (values[0] >= 50 && values[1] >= 50 && values[2] >= 50 && values[3] >= 50 && values[4] >= 20);
}

bool isLetterP(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] < 10);
}

bool isLetterQ(int values[]) {
  return (values[0] < 40 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] < 10);
}

bool isLetterR(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterS(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterT(int values[]) {
  return (values[0] > 100 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterU(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterV(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterW(int values[]) {
  return (values[0] < 40 && values[1] < 40 && values[2] < 40 && values[3] > 100 && values[4] > 40);
}

bool isLetterX(int values[]) {
  return (values[0] > 90 && values[1] > 100 && values[2] > 100 && values[3] > 100 && values[4] > 40);
}

bool isLetterY(int values[]) {
  return (values[0] > 50 && values[1] > 50 && values[2] > 50 && values[3] < 40 && values[4] < 50);
}

