#include "Arduino.h"
INLINE void line_read(){
        line_update();
        distance_from_origin();
        pid.Compute();
}
/********************line update********************/
INLINE void line_update(){
        /*  for(uint8_t i=0; i<7; i++) {
                  line.sensor[i] = analogRead(line.channel[i]);
                  //Serial.println(line.sensor[i]);
           }*/
           #ifdef Daisen

        line.sensor[2] = analogRead(CN1);
        line.sensor[3] = analogRead(CN2);
        line.sensor[4] = analogRead(CN3);

        for(uint8_t i=0; i<5; i++) {
                if(line.sensor[i] > line.Silver_Threshold[i]) {
                        line.OnSilver[i] = 1;
                }else{
                        line.OnSilver[i] = 0;
                }
        }

        for(uint8_t i=0; i<5; i++) {
                if(line.sensor[i] > line.White_Threshold[i]) {
                        line.OnWhite[i] = 1;
                        line.OnBlack[i] = 0;
                }else{
                        line.OnWhite[i] = 0;
                        line.OnBlack[i] = 1;
                }
        }

           #else
        line.sensor[0] = analogRead(A6);
        line.sensor[1] = analogRead(A5);
        line.sensor[2] = analogRead(A4);
        line.sensor[3] = analogRead(A3);
        line.sensor[4] = analogRead(A2);
        line.sensor[5] = analogRead(A1);
        line.sensor[6] = analogRead(A0);

        for(uint8_t i=0; i<7; i++) {
                if(line.sensor[i] > line.Silver_Threshold[i]) {
                        line.OnSilver[i] = 1;
                }else{
                        line.OnSilver[i] = 0;
                }
        }

        for(uint8_t i=0; i<7; i++) {
                if(line.sensor[i] > line.White_Threshold[i]) {
                        line.OnWhite[i] = 1;
                        line.OnBlack[i] = 0;
                }else{
                        line.OnWhite[i] = 0;
                        line.OnBlack[i] = 1;
                }
        }
           #endif
}
/********************line print********************/
INLINE void line_print(){
        for(uint8_t i=0; i<7; i++) {
                Serial.print(line.sensor[i]);
                Serial.print(",");
        }
        Serial.print("\r\n");
}
/********************PD for line********************/
INLINE void distance_from_origin(){
  #ifdef Daisen
        uint8_t n=0;
        line.Origin_Distance = 0;
        if(line.OnBlack[2] == 1) {
                line.Origin_Distance += 28;
                n++;
        }
        if(line.OnBlack[3] == 1) {
                line.Origin_Distance += 0;
                n++;
        }
        if(line.OnBlack[4] == 1) {
                line.Origin_Distance += -28;
                n++;
        }
        if(n!=0) {
                line.Origin_Distance = line.Origin_Distance / n;
        }
  #else
        uint8_t n=0;
        line.Origin_Distance = 0;
        if(line.OnBlack[0] == 1) {
                line.Origin_Distance += 50;
                n++;
        }
        if(line.OnBlack[1] == 1) {
                line.Origin_Distance += 35;
                n++;
        }                    //*/
        if(line.OnBlack[2] == 1) {
                line.Origin_Distance += 28;
                n++;
        }
        if(line.OnBlack[3] == 1) {
                line.Origin_Distance += 0;
                n++;
        }
        if(line.OnBlack[4] == 1) {
                line.Origin_Distance += -28;
                n++;
        }
        if(line.OnBlack[5] == 1) {
                line.Origin_Distance += -35;
                n++;
        }
        if(line.OnBlack[6] == 1) {
                line.Origin_Distance += -50;
                n++;
        } //*/
        if(n!=0) {
                line.Origin_Distance = line.Origin_Distance / n;
        }
  #endif
}
/********************linetrace********************/
INLINE void linetrace(int mpower){
        //line.Motorleft = mpower-line.Origin_Distance*_KP;
        //line.Motorright = mpower+line.Origin_Distance*_KP;
        line.Motorleft = mpower+line.Output;// - 6;
        line.Motorright = mpower-line.Output;
        MD.motor(line.Motorleft,line.Motorright);
}
INLINE void linetrace_onof(){
        if(line.OnWhite[2]&&line.OnWhite[4]) {
                MD.motor(300,300);
        }else if(line.OnBlack[2]) {
                MD.motor(0,80);
        }else if(line.OnBlack[4]) {
                MD.motor(80,0);
        }
}
/********************LEDs********************/
INLINE void LED(bool led1,bool led2,bool led3,bool led4){
        digitalWrite(LED1,led1);
        digitalWrite(LED2,led2);
        digitalWrite(LED3,led3);
        digitalWrite(LED4,led4);
}
/********************Buzzer********************/
INLINE void Buzzer(uint8_t mode){
        switch (mode) {
        case 0:
                tone(buzzer, 3000, 50); delay(30); break;
        case 1:
                tone(buzzer,3000, 50); delay(20); tone(buzzer,6000, 50); delay(20); break;
        case 2:
                tone(buzzer,10000, 50); delay(20); tone(buzzer,4200, 50); delay(20); break;
        case 3:
                tone(buzzer,9000, 100); delay(100); tone(buzzer,7200, 100); delay(100);
                tone(buzzer,1200, 100); delay(100); tone(buzzer,8000, 100); delay(100); break;
        }
}
/********************BNO055 Gyro********************/
INLINE int getdig(){
        int dig = 0;
        imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);//0x1Aから2byte
        dig = euler.x() - sensors.flont_dir;
        if(dig > 180) {
                dig -= 360;
        }else if(dig < -179) {
                dig +=360;
        }   //*/
        return dig;
}
INLINE void setflont(int set){
        imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);//0x1Aから2byte
        sensors.flont_dir = euler.x() + set;
}
INLINE void Spin_to_flont(int power1,int power2){
        sensors.dir = getdig();
        BNOPD.dt = (micros() - BNOPD.preTime)/1000000;
        BNOPD.preTime = micros();
        //BNOPD.I += sensors.dir * BNOPD.dt;
        BNOPD.OUT = sensors.dir*BNOPD.kp + ((sensors.dir-BNOPD.previous_val)/BNOPD.dt)*BNOPD.kd;
        /*if(BNOPD.OUT < 5&& BNOPD.OUT > -5) {
                BNOPD.OUT = 0;
           }*/
        MD.motor((-BNOPD.OUT + power1),(BNOPD.OUT + power2));
}
INLINE void Spin_to_flont2(int mode){
        while(1) {
                sensors.dir = getdig();
                if(sensors.dir <= 10 && sensors.dir >= -10) {
                        MD.motor(0,0);
                        break;
                }else if(sensors.dir > 10) {
                        //右側
                        MD.motor(-mode, mode);
                }else if(sensors.dir < -10 ) {
                        //左側
                        MD.motor(mode, -mode);
                }
        }
}
/********************PSD sensor********************/
INLINE double PSDread() {
        double distance;
        distance = 26.549*pow(5.0*(analogRead(PSD))/1024,-1.2091);
        return distance;
}
INLINE double USread(uint8_t n) {
        double distance;
        int duration = 0;
        // 10マイクロ秒出力 -> パルスが送信
        digitalWrite(TRIG[n], HIGH);
        delayMicroseconds(11);
        digitalWrite(TRIG[n], LOW);
        // パルス送信から受信までの時間を取得
        duration = pulseIn(ECHO[n], HIGH,17000);
        if (duration > 0) {
                // 往復にかかった時間 / 2 * 音速(340m/s)
                distance = (double)duration / 2 * 340  / 10000;
        }else{
                distance = 289;
        }
        return distance;
}
/********************IRsensor********************/
INLINE void IRread(int error_angle){
        //float h = 0.5;
        digitalWrite(Ball, HIGH); //センサー用マイコンの割り込みピンにHIGHを贈る
        delayMicroseconds(15);
        digitalWrite(Ball, LOW); //センサー用マイコンの割り込みピンをLOWに戻す
        delayMicroseconds(300); //割り込み発生させてから300us待つ

        while(BALL.available()) { //センサー用マイコンからの送信データをチェック
                if ( BALL.read() == 'H' ) {
                        data.lowbyte = BALL.read(); // 下位バイトの読み取り
                        data.highbyte = BALL.read(); // 上位バイトの読み取り
                        data.intdat = data.highbyte*256+data.lowbyte;//復元
                }
        }
        /*
           sensor.IRlong
           5:とても近い
           4:近い
           3:遠い
           2:ボールなし
         */
        if(data.intdat > 5000) {
                sensors.IRangle = data.intdat - 5000;
                sensors.IRlong = 5;
        }else if(data.intdat > 4000) {
                sensors.IRangle = data.intdat - 4000;
                sensors.IRlong = 4;
        }else if(data.intdat > 3000) {
                sensors.IRangle = data.intdat - 3000;
                sensors.IRlong = 3;
        }else if(data.intdat == 2000 || data.intdat == 0) {
                sensors.IRangle = 5000;
                sensors.IRlong = 2;
        }
        if(sensors.IRangle % 1000 == 0&&sensors.IRangle != 5000) {
                sensors.IRangle = 0;
        }
        if(sensors.IRangle > 180 && sensors.IRangle != 5000) {
                sensors.IRangle = sensors.IRangle - 360;
                //sensors.IRangle -= 23;
        }
        if(sensors.IRangle != 5000) {
                //移動平均
                for(int i = 0; i < (average_num); i++) {
                        l[i] = l[i+1];
                }
                l[(average_num-1)] = sensors.IRangle;
                for (int i = 0; i < (average_num); i++) {
                        sensors.IRangle +=l[i];
                }
                sensors.IRangle = sensors.IRangle/average_num;
                //移動平均
        }
        if(sensors.IRangle > (-1*error_angle) &&  sensors.IRangle < error_angle) {
                sensors.IRangle = 0;
        }
}
/********************Sensors********************/
INLINE void UPDATE(){
        //line_update();
        line_read();
        sensors.dir = getdig();
        sensors.lineB = analogRead(CN4);
        sensors.US = USread(0);
        sensors.US2 = USread(1);
}
INLINE void PCprint(){
        /*pc.print("Line 1\t");
           pc.print(line.sensor[2]);
           pc.print("\tLine 2\t");
           pc.print(line.sensor[3]);
           pc.print("\tLine 3\t");
           pc.print(line.sensor[4]);
           pc.print("\tLine B\t");
           pc.print(sensors.lineB);
           pc.print("\r\n");*/
        pc.print("Line 1\t");
        pc.print(line.sensor[0]);
        pc.print("\tLine 2\t");
        pc.print(line.sensor[1]);
        pc.print("\tLine 3\t");
        pc.print(line.sensor[2]);
        pc.print("\tLine 4\t");
        pc.print(line.sensor[3]);
        pc.print("\tLine 5\t");
        pc.print(line.sensor[4]);
        pc.print("\tLine 6\t");
        pc.print(line.sensor[5]);
        pc.print("\tLine 7\t");
        pc.print(line.sensor[6]);
        pc.print("\tLine distance\t");
        pc.print(line.Origin_Distance);
        pc.print("\tLine B\t");
        pc.print(sensors.lineB);
        pc.print("\tUS\t");
        pc.print(sensors.US);
        pc.print("cm");
        pc.print("\tUS2\t");
        pc.print(sensors.US2);
        pc.print("cm");
        pc.print("\r\n");

}
/********************Soccer Program********************/
INLINE void Soccer(){
        IRread(20);
        sensors.psd = PSDread();
        if(sensors.IRangle == 0) {//正面
                if(sensors.psd < 20.0) {
                        MD.motor(-100,-100);
                }else{
                        MD.motor(300,300);
                }

        }else if(sensors.IRangle < 180 && sensors.IRangle >=20) {//右側
                MD.motor(100,-100);
                movement_flag = RSPIN;
        }else if(sensors.IRangle > -180&& sensors.IRangle < -20) {//左側
                MD.motor(-100,100);
                movement_flag = LSPIN;
        }else if(sensors.IRangle == 5000) {//ない
                if(movement_flag == LSPIN || movement_flag == RSPIN) {
                        move(movement_flag,100);
                }else{
                        move(RSPIN,100);
                }
        }
        //delay(50);
        pc.println(sensors.IRangle);
}
INLINE void Soccer2(){
        IRread(20);
        sensors.dir = getdig();
        if(sensors.IRangle == 0) {
                MD.motor(300,300);
        }else if(sensors.IRangle == 5000) {
                sensors.psd = PSDread();
                if(sensors.psd <= 20.0 && sensors.psd >= 10.0) {
                        MD.motor(0,0);
                }else if(sensors.psd > 20.0) {
                        Spin_to_flont(-200,-200);
                }else if(sensors.psd < 10.0) {
                        Spin_to_flont(100,100);
                }
        }else if(sensors.IRangle < 90 && sensors.IRangle >=20) {//右側
                MD.motor(100,-100);
        }else if(sensors.IRangle > -90&& sensors.IRangle < -20) {//左側
                MD.motor(-100,100);
        }else if(sensors.IRangle < 180 && sensors.IRangle >=90) {//右側
                MD.motor(-300,-300);
        }else if(sensors.IRangle > -180 && sensors.IRangle < -90) {//左側
                MD.motor(-300,-300);
        }
        pc.println(sensors.IRangle);
}
//*/
/********************Move funcsion********************/
INLINE void move(int mode,int power){
        switch (mode) {
        case FW:
                MD.motor(power, power);
                break;
        case BK:
                MD.motor(-power, -power);
                break;
        case RSPIN:
                MD.motor(power, -power);
                break;
        case LSPIN:
                MD.motor(-power, power);
                break;
        case STOP:
                MD.motor(power, power);
                break;
        }
}
INLINE void HOME_TO_MAIN(){
        sensors.SWdre = Pusshing_duration();
        if(sensors.SWdre > 500) {
                MODE = 'Z';
        }else{
                MODE = 'A';
        }
        LED(0,0,0,0);
}
INLINE void HOME(){
        if(ReadSWstate2() == 1) {
                setflont(0);
                LED(0,0,1,0);
        }else{
                LED(0,0,0,0);
        }
        if(ReadpusshingTimes2() > 3) {
                SetpusshingTimes2(0);
        }else{
                SRC_LAP = ReadpusshingTimes2();
        }

        if(Timer.read_ms() > 650) {
                led.L4 =!led.L4;
                Timer.reset();
        }
        digitalWrite(LED4,led.L4);
}
