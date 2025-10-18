#include "HardwareSoftwareConfig.h"
#include "LEDs.h"
#define MAX_TEXT_LINES 15

extern String arrText[MAX_TEXT_LINES];
void textToMatrix(const char *text);
void printMatrix();