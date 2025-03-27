#include <float.h>
#include "infoProcessing.h"

#define MAX_WORD_LENGTH 10

//Mappings for each of 26 letters of alphabet, 5 fingers.
//If -200 for a value it means that it is not implemented yet.
//NOTE: MAKE SURE THIS SET IS COMPATIBLE WITH MAX_CALIBRATED_VALUE AND THE DEFINED FINGER ORDER.
int letterMappings[26][5] = {
  {45, 55, 65, 60, 2}, //a
  {0, 0, 0, 0, 30}, //b
  {0, 10, 17, 0, 0}, //c
  {40, 40, 40, 0, 5}, //d
  {30, 30, 50, 15, 30}, //e
  {0, 0, 0, 20, 20}, //f
  {30, 40, 30, 0, 10}, //g
  {40, 40, 0, 0, 10}, //h
  {0, 30, 21, 30, 10}, //i
  {50, 50, 20, 0, 100}, //j
  {30, 30, 0, 0, 10}, //k
  {40, 30, 10, 0, 0}, //l
  {15,  35, 0, 0, 100}, //m
  {2, 10, 38, 18, 100}, //n
  {30, 30, 20, 10, 10}, //o
  {20, 20, 0, 0, 0}, //p
  {30, 30, 35, 0, 20}, //q
  {20, 27, 0, 5, 10}, //r
  {33, 50, 70, 70, 2}, //s
  {30, 30, 35, 0, 20}, //t
  {30, 30, 35, 0, 20}, //u
  {-200, -200, -200, -200, -200}, //v
  {20, 0, 0, 0, 0}, //w
  {10, 5, 5, 0, 20},//x
  {0, 15, 15, 5, 0}, //y
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
  float minDistance = FLT_MAX;
  if(!reading.isValid()) { 
    Serial.println("Invalid reading!");
    return produced;}

  for(int l = 0; l < 26; l++) {
    float distance = reading.distToVector(letterMappings[l]);
    if(distance < minDistance) {
        produced = 'A' + l;
        minDistance = distance;
    }
  }

  for(int s = 0; s < 1; s++) {
    float distance = reading.distToVector(specialCharacters[s]);
    if(distance < minDistance) {
        produced = specialCharacters[s][5];
        minDistance = distance;
    }
  }

  if(produced == 'I' && jMotion) { produced = 'J';}
  else if(produced == 'D' && zMotion) { produced = 'Z';}

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