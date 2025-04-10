#include "Adafruit_Sensor.h"
#include <float.h>
#include "infoProcessing.h"

#define MAX_WORD_LENGTH 10

//Mappings for each of 26 letters of alphabet, 5 fingers.
//If -200 for a value it means that it is not implemented yet.
//NOTE: MAKE SURE THIS SET IS COMPATIBLE WITH MAX_CALIBRATED_VALUE AND THE DEFINED FINGER ORDER.
int letterMappings[52][5] = {
  {0, 10, 60, 20, 2}, //a
  {20, 55, 90, 50, 50},
  {0, 0, 0, 0, 30}, //b
  {20, 20, 20, 20, 100}, //b
  {0, 10, 17, 0, 0}, //c
  {20, 30, 40, 20, 10}, //c
  {30, 30, 30, 0, 0}, //d
  {50, 50, 50, 10, 20}, //d
  {20, 30, 40, 15, 30}, //e
  {40, 40, 55, 25, 50}, //e
  {0, 0, 0, 10, 10}, //f
  {15, 15, 15, 25, 25}, //f
  {20, 20, 20, 0, 0}, //g
  {80, 80, 80, 15, 20}, //g
  {30, 30, 0, 0, 10}, //h
  {60, 60, 15, 15, 25}, //h
  {0, 20, 20, 25, 0}, //i
  {0, 50, 40, 40, 20}, //i
  {0, 20, 20, 25, 0}, //j
  {0, 50, 40, 40, 20}, //j
  {30, 30, 0, 0, 10}, //k
  {30, 30, 0, 0, 10}, //k
  {40, 30, 10, 0, 0}, //l
  {40, 30, 10, 0, 0}, //l
  {15,  35, 0, 0, 100}, //m
  {15,  35, 0, 0, 100}, //m
  {2, 10, 38, 18, 100}, //n
  {2, 10, 38, 18, 100}, //n
  {30, 30, 20, 10, 10}, //o
  {30, 30, 20, 10, 10}, //o
  {20, 20, 0, 0, 0}, //p
  {20, 20, 0, 0, 0}, //p
  {30, 30, 35, 0, 20}, //q
  {30, 30, 35, 0, 20}, //q
  {20, 27, 0, 5, 10}, //r
  {20, 27, 0, 5, 10}, //r
  {33, 50, 70, 70, 2}, //s
  {33, 50, 70, 70, 2}, //s
  {30, 30, 35, 0, 20}, //t
  {30, 30, 35, 0, 20}, //t
  {30, 30, 35, 0, 20}, //u
  {30, 30, 35, 0, 20}, //u
  {-200, -200, -200, -200, -200}, //v
  {-200, -200, -200, -200, -200}, //v
  {20, 0, 0, 0, 0}, //w
  {20, 0, 0, 0, 0}, //w
  {10, 5, 5, 0, 20},//x
  {10, 5, 5, 0, 20},//x
  {0, 15, 15, 5, 0}, //y
  {0, 15, 15, 5, 0}, //y
  {50, 50, 20, 0, 100}, //z
  {50, 50, 20, 0, 100} //z
};

int specialCharacters[1][6] ={ 
  {0, 0, 0, 0, 0, ' '}
};

static char pipedWord[MAX_WORD_LENGTH + 1]; //+1 to account for null character
static char* curCharPtr = &pipedWord[0];
extern bool jMotion;
extern bool zMotion;

char produceChar(SensorReading &reading) {
  reading.print();
  
  char produced = -1;
  std::vector<char> candidates;
  if(!reading.isValid()) { 
    Serial.println("Invalid reading!");
    return produced;}

  for(int l = 0; l < 26; l++) {
    bool isCandidate = true;
    for(int f = 0; f < 5; f++) {
      float lowBound = letterMappings[l*2][f];
      float highBound = letterMappings[l*2 + 1][f];
      if(reading.readingVals[f] < lowBound || reading.readingVals[f] > highBound) { isCandidate = false; break;}
    }

    if(isCandidate) {candidates.push_back('A' + l);}
  }

  for(int s = 0; s < 1; s++) {
    float distance = reading.distToVector(specialCharacters[s]);
    if(distance < 300) {
      candidates.push_back(specialCharacters[s][5]);
    }
  }

  if(candidates.size() == 0){
    Serial.println("No candidates found!");
    return produced;
  }

  float minDistance = FLT_MAX;
  Serial.printf("Candidates: ");
  for(int c = 0; c < candidates.size(); c++) {
    char chr = candidates[c];
    if(chr == ' ') {continue;} //Special characters handled separately

    Serial.printf("%c ", chr);
    int lIndex = chr - 'A';
    int midpoint[5];
    for(int f = 0; f < 5; f++){
      midpoint[f] = letterMappings[lIndex * 2][f] + letterMappings[lIndex * 2 + 1][f];
      midpoint[f] /= 2;
    }
    float distance = reading.distToVector(midpoint);
    if(distance < minDistance) { produced = chr; minDistance = distance;}
  }
  Serial.println();

  //Some letters are differentiable only by gyroscope
  if(produced == 'I' || produced == 'J') { produced = jMotion ? 'J' : 'I';}
  else if(produced == 'D' || produced == 'Z') { produced = zMotion ? 'Z' : 'D';}

  return produced;
}

char* produceWord(char appendedChar) {
  *curCharPtr = appendedChar;
  curCharPtr++;

  if(curCharPtr == &pipedWord[MAX_WORD_LENGTH] || appendedChar == ' ') {
    *curCharPtr = '\0';
    curCharPtr = &pipedWord[0];
    return curCharPtr;
  }

  return nullptr;
}