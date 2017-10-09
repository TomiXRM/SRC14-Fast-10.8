#ifndef SRC_SSWITCHWITCH_MANAGEMENT_H
#define SRC_SSWITCHWITCH_MANAGEMENT_H

#include "Arduino.h"

void SetPin(int _pin);
void pusshingTimes();
void switchingSWFLAG();
void SetpusshingTimes(int num);
int ReadpusshingTimes();
unsigned long Pusshing_duration();
bool SWread();
bool ReadSWstate();
void SetSWstate(bool a);
#endif