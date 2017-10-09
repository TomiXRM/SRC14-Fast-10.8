#include "Arduino.h"               //Arduino
#include "Adafruit_Sensor.h"       //Sensor
#include "Adafruit_BNO055.h"       //BNO055
#include "utility/imumaths.h"      //BNO055
#include "timer.h"                 //タイマー
#include "MotorDriver.h"           //モータードライバー
#include "VarSpeedServo.h"         //サーボ
#include "SRC_Switch_management.h" //スイッチ
#include "PID_v1.h"                //PID
#include "Wire.h"                  //I2C
#include "string.h"

#define INLINE inline __attribute__((always_inline))
#define wait_ms delay
#define wait_us delayMicroseconds
#define average_num 40

#define Rsilver 0
#define Lsilver 1

#define FIRST_LAP   1
#define SECOND_LAP  2
#define THIRD_LAP   3

#define White_Threshold0 300
#define White_Threshold1 300
#define White_Threshold2 300
#define White_Threshold3 300
#define White_Threshold4 300
#define White_Threshold5 300
#define White_Threshold6 300

#define Silver_Threshold0 750
#define Silver_Threshold1 750
#define Silver_Threshold2 750
#define Silver_Threshold3 750
#define Silver_Threshold4 750
#define Silver_Threshold5 750
#define Silver_Threshold6 750

#define buzzer 9
#define ServoM 12
#define TX2 16
#define RX2 17
#define Sw2 19
#define Sw1 18
#define SWLED1 23
#define SWLED2 22
#define Ball 28
#define PSD A8
#define LED1 32
#define LED2 33
#define LED3 34
#define LED4 35
#define trig 42
#define echo 43
#define trig2 40
#define echo2 41

#define FW 1
#define LSPIN 2
#define RSPIN 3
#define STOP 4
#define BK 5

#define pc Serial
#define BALL Serial2
#define CN1 A7
#define CN2 A8
#define CN3 A9
#define CN4 A10
#define CN5 A11

//#define TESTMODE
#define Daisen
/********************************************************変数********************************************************/
char MODE = 'A';
int movement_flag = RSPIN;//初期値
int l[average_num];
int SRC_LAP = 0;
const uint8_t TRIG[2] = {trig,trig2};
const uint8_t ECHO[2] = {echo,echo2};
struct {
  #ifdef Daisen
        const int channel[7]= {A6,A5,CN1,CN2,CN3,A1,A0};
  #else
        const int channel[7]= {A6,A5,A4,A3,A2,A1,A0};
  #endif
        int sensor[7] = {0,0,0,0,0,0,0};
        const int White_Threshold[7]={
                White_Threshold0,
                White_Threshold1,
                White_Threshold2,
                White_Threshold3,
                White_Threshold4,
                White_Threshold5,
                White_Threshold6
        };
        const int Silver_Threshold[7]={
                Silver_Threshold0,
                Silver_Threshold1,
                Silver_Threshold2,
                Silver_Threshold3,
                Silver_Threshold4,
                Silver_Threshold5,
                Silver_Threshold6
        };
        double Origin_Distance;
        bool OnSilver[7];
        bool OnBlack[7];
        bool OnWhite[7];
        int Silver_count[3];
        int Motorleft;
        int Motorright;
        double Setpoint = 0.0;
        double Input;
        double Output;

        double Kp=5.4;
        double Kd=1.4;
        double Ki=0;
        /*パラメーター
           power 150
           Kp    5
           Kd    1.7
           Ki    0.5

           power 150
           Kp    13
           Kd    1.7
           Ki    0.5

           power 140
           Kp    13
           Kd    1.7
           Ki    0.5

           power 100
           Kp    8
           Kd    1.2
           Ki    0.5
         */
} line;
struct {
        int lineB;
        int flont_dir;
        int dir;
        int US;
        int US2;
        double psd;
        unsigned long SWdre;
        unsigned long SW2dre;
        int IRangle;
        int IRlong;
} sensors;
struct {
        uint8_t highbyte;
        uint8_t lowbyte;
        short intdat;
} data;
struct {
        float kd = 2;
        float ki = 0.1;
        float kp = 3;
        int previous_val;
        int OUT;
        float dt;
        int preTime;
        int I;
} BNOPD;
struct {
        bool L1;
        bool L2;
        bool L3;
        bool L4;
} led;


/********************************************************設定********************************************************/
MotorDriver MD(25,24,27,26);
VarSpeedServo myservo;
timer Timer;
Adafruit_BNO055 bno = Adafruit_BNO055();
PID pid(&line.Origin_Distance, &line.Output, &line.Setpoint, line.Kp, line.Ki, line.Kd,DIRECT);

void setup(){
        pc.begin(230400);
        BALL.begin(115200);
        Wire.begin(400);
        pid.SetMode(AUTOMATIC);
        pid.SetOutputLimits(-300,300);
        pid.SetSampleTime(7);
        MD.init(0);
        pinMode(LED1,OUTPUT);//LEDs
        pinMode(LED2,OUTPUT);//LEDs
        pinMode(LED3,OUTPUT);//LEDs
        pinMode(LED4,OUTPUT);//LEDs
        pinMode(SWLED1,OUTPUT);//SWled2
        pinMode(SWLED2,OUTPUT);//SWled2
        pinMode(buzzer,OUTPUT);//buzzer]
        pinMode(13,OUTPUT);//led
        pinMode(Ball,OUTPUT);
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        pinMode(trig2, OUTPUT);
        pinMode(echo2, INPUT);
        SetPin(Sw1);
        SetPin2(Sw2);
        myservo.attach(ServoM);
        myservo.write(180);
        digitalWrite(SWLED1,1);
        digitalWrite(SWLED2,1);
        Buzzer(0);
        Buzzer(1);
        Buzzer(2);

        pc.println(F("Hello world!!SRC14 Robot made by TomiXRM"));
        pc.println(F("Please calibration this Robot!!"));

        if (!bno.begin()) {
                Serial.println(F("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
        }
        bno.setExtCrystalUse(true);
        /*
           uint8_t system, gyro, accel, mag;
           system = gyro = accel = mag = 0;
           bno.getCalibration(&system, &gyro, &accel, &mag);
           LED(1,0,0,0);
           while (mag != 3) bno.getCalibration(&system, &gyro, &accel, &mag); //キャリブレーション完了まで待つ*/
        LED(0,0,0,1);
        Buzzer(3);
        LED(0,0,0,0);

        pc.println(F("Login successed!!"));

        led.L4 = 0;
        setflont(0);
        sensors.IRangle = 0;
        for (uint8_t i = 0; i < average_num; i++) {
                l[i] = 0;
        }
        SetSWstate(0);
        SetSWstate2(0);
}


void loop(){
        MD.motor(0,0);
        /****************************************HOME****************************************/
        while(ReadSWstate() == 0) {
                HOME();
        }
        HOME_TO_MAIN();
        switch (MODE) {
        case 'A'://ノーマル
        #ifdef TESTMODE
                MAIN_TEST(); break;
        #else
                MAIN(); break;
        #endif

        case 'Z'://長押し
                Buzzer(3);
                led.L3 = 0;
                while(ReadSWstate() == 1) {
                        UPDATE();
                        PCprint();
                        //line_print();
                        //wait_ms(0);
                        LED(0,0,(!led.L3),0);
                }
                break;
        }
        MD.motor(0,0);
        SetSWstate(0);
        SetSWstate2(0);
        delay(200);
}
