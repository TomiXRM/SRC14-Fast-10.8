#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "Arduino.h"

class MotorDriver {
public:
MotorDriver(unsigned char INA1,unsigned char INB1,unsigned char INA2,unsigned char INB2);
MotorDriver();  // Default pin selection.
void init();  // Initialize TIMER 1, set the PWM to 20kHZ.
void setM1Speed(int speed);     // Set speed for M1.
void setM2Speed(int speed);     // Set speed for M2.
void motor(int m1Speed, int m2Speed);     // Set speed for both M1 and M2.
void setM1Brake(int brake);     // Brake M1.
void setM2Brake(int brake);     // Brake M2.
void Brakes(int m1Brake, int m2Brake);     // Brake both M1 and M2.
void Lock(uint8_t mode);    //tomix original
private:
unsigned char _INA1;
unsigned char _INB1;
static const unsigned char _PWM1 = 7;
unsigned char _EN1DIAG1;
unsigned char _CS1;
unsigned char _INA2;
unsigned char _INB2;
static const unsigned char _PWM2 = 8;
unsigned char _EN2DIAG2;
unsigned char _CS2;
uint8_t move = 0;

};
#endif
