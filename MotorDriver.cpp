#include "MotorDriver.h"

MotorDriver::MotorDriver()
{
  //Pin map
  _INA1 = 2;
  _INB1 = 4;
  _INA2 = 7;
  _INB2 = 8;
}
MotorDriver::MotorDriver(unsigned char INA1,unsigned char INB1,unsigned char INA2,unsigned char INB2){
  _INA1 = INA1;
  _INB1 = INB1;
  _INA2 = INA2;
  _INB2 = INB2;
}


void MotorDriver::init()
{
// Define pinMode for the pins and set the frequency for timer1.

  pinMode(_INA1,OUTPUT);
  pinMode(_INB1,OUTPUT);
  pinMode(_PWM1,OUTPUT);
  pinMode(_INA2,OUTPUT);
  pinMode(_INB2,OUTPUT);
  pinMode(_PWM2,OUTPUT);
}

void MotorDriver::setM1Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;
  analogWrite(_PWM1,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
  if (speed == 0)
  {
    digitalWrite(_INA1,LOW);   // Make the motor coast no
    digitalWrite(_INB1,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_INA1,LOW);
    digitalWrite(_INB1,HIGH);
  }
  else
  {
    digitalWrite(_INA1,HIGH);
    digitalWrite(_INB1,LOW);
  }
}

void MotorDriver::setM2Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 400)  // Max 
    speed = 400;
  analogWrite(_PWM2,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
  if (speed == 0)
  {
    digitalWrite(_INA2,LOW);   // Make the motor coast no
    digitalWrite(_INB2,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_INA2,LOW);
    digitalWrite(_INB2,HIGH);
  }
  else
  {
    digitalWrite(_INA2,HIGH);
    digitalWrite(_INB2,LOW);
  }
}

void MotorDriver::motor(int m1Speed, int m2Speed)
{
  if(move == 1){
    setM1Speed(0);
    setM2Speed(0);
  }else{
    if(m1Speed == 0){
        setM1Brake(400);

    }else{
          setM1Speed(m1Speed);
    }
    if(m2Speed == 0){
        setM2Brake(400);

    }else{
        setM2Speed(m2Speed);
    }
  }
}

void MotorDriver::setM1Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA1, LOW);
  digitalWrite(_INB1, LOW);
  analogWrite(_PWM1,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
}

void MotorDriver::setM2Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA2, LOW);
  digitalWrite(_INB2, LOW);
  analogWrite(_PWM2,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
}

void MotorDriver::Brakes(int m1Brake, int m2Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
}


void MotorDriver::Lock(uint8_t mode){
  move = mode;
}