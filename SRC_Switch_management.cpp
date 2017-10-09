#include "SRC_Switch_management.h"


volatile int count;
unsigned long tm;
int pin;
volatile bool SWFLAG;
void SetPin(int _pin){
	pin = _pin;
	pinMode(pin,INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pin), pusshingTimes, RISING );
	attachInterrupt(digitalPinToInterrupt(pin), switchingSWFLAG, FALLING );
	count = 0;
	SWFLAG = 0;
	tm = 0;
}

void pusshingTimes(){
  count += 1;
}

void switchingSWFLAG(){
	SWFLAG = !SWFLAG;
}

int ReadpusshingTimes(){
	return count;
}

void SetpusshingTimes(int num){
	count = num;
}

unsigned long Pusshing_duration(){
	detachInterrupt(digitalPinToInterrupt(pin));
	tm = millis();
	//while(digitalRead(pin) == 1);
	while(digitalRead(pin) == 0){

	}
	attachInterrupt(digitalPinToInterrupt(pin), pusshingTimes, RISING);
	attachInterrupt(digitalPinToInterrupt(pin), switchingSWFLAG, FALLING );
	return (millis() - tm);
}

bool SWread(){
	return digitalRead(pin);
}

bool ReadSWstate(){
	return SWFLAG;
}

void SetSWstate(bool a){
	SWFLAG = a;
}