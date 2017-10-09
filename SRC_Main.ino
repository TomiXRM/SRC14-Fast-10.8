#include "Arduino.h"
#ifdef Daisen
#else
#endif
/********************メインプログラム********************/
INLINE void MAIN(){
        //SRC_LAP = 3;//
        while(ReadSWstate() == 1) {
                if(SRC_LAP == 0) {
                        while(line.OnSilver[3] == 0) {
                                line_read();
                                linetrace(70);
                        }
                        SRC_LAP++;
                }else if(SRC_LAP == 1) {
                        /****************************************1週目****************************************/
                        SpaceCentre_To_Miningpit();
                        miningpit();
                        Miningpit_To_Moonbase1();
                        moonbase1();
                        //Moonbase1_TO_Moonbase2();
                        avoid_container();
                        while(line.OnSilver[3] == 0) {
                                line_read();
                                linetrace(70);
                        }
                        SRC_LAP++;
                }else if(SRC_LAP == 2) {
                        /****************************************2週目****************************************/
                        SpaceCentre_To_Miningpit();
                        miningpit();
                        Miningpit_To_Moonbase1();
                        Moonbase1_TO_Moonbase2();
                        moonbase2();
                        avoid_container();
                        while(line.OnSilver[3] == 0) {
                                line_read();
                                linetrace(70);
                        }
                        /*MD.motor(0,0);
                           Buzzer(3);
                           while(1) ;*/
                        SRC_LAP++;
                }else if(SRC_LAP == 3) {
                        /****************************************3週目****************************************/
                        SpaceCentre_To_Miningpit();
                        miningpit();
                        Miningpit_To_Moonbase1();
                        Moonbase1_TO_Moonbase2();
                        goto_moonbase3();
                        moonbase3();
                }else{
                        line_read();
                        linetrace(70);
                }
        }
}

INLINE void MAIN_TEST(){
        while(ReadSWstate() == 1) {
                avoid_container();
        }
}
/****************************************スタートからコンテナ運び出しまで****************************************/
INLINE void SpaceCentre_To_Miningpit(){
        myservo.write(120);
        while(1) {
                line_read();
                if(line.OnSilver[3]) {
                        MD.motor(0,0);
                        Buzzer(0);
                        delay(1000);
                        MD.motor(140,140);
                        delay(120);
                        #ifdef Daisen
                }else if(line.OnSilver[4]) {
                        #else
                }else if(line.OnSilver[4]&&line.OnSilver[5]) {
                        #endif

                        //miningpit
                        MD.motor(0,0);
                        delay(30);
                        MD.motor(-200,-200);
                        delay(70);//30
                        MD.motor(20,-20);
                        delay(25);
                        LED(0,0,0,1);
                        break;
                }else{
                        linetrace(54);
                }
        }
}
/****************************************運びだしからムーンベース１まで***************************************/
INLINE void Miningpit_To_Moonbase1(){
        Timer.reset();
        while(Timer.read_ms() < 350) {
                line_read();
                linetrace(90);
        }
        while(line.OnSilver[0] == 0&&line.OnSilver[1] == 0&&line.OnSilver[2] == 0) {
                line_read();
                linetrace(60);
        }
}
/****************************************ムーンベース１からムーンベース２まで****************************************/
INLINE void Moonbase1_TO_Moonbase2(){
        Timer.reset();
        while(Timer.read_ms() < 700) {
                line_read();
                linetrace(90);
        }
        #ifdef Daisen
        while(line.OnSilver[2] == 0) {
                line_read();
                linetrace(50);
        }
        #else
        while(line.OnSilver[0] == 0&&line.OnSilver[1] == 0&&line.OnSilver[2] == 0) {
                line_read();
                linetrace(50);
        }
        #endif
}
/****************************************ムーンベース３に行くプログラム****************************************/
INLINE void goto_moonbase3(){
        line_update();
        while(line.OnBlack[2] == 0) {
                line_update();
                MD.motor(40,0);
        }
        MD.motor(-40,0);
        delay(200);
        MD.motor(0,0); delay(600);
        MD.motor(80,80); delay(700);
        LED(1,0,0,0);
        line_update();
        MD.motor(0,0);
        while(line.OnWhite[3] && line.OnWhite[2] && line.OnWhite[4]) {
                line_update();
                MD.motor(60,60);
        }
        LED(0,1,0,0);
        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        LED(0,0,1,0);
        while(line.OnSilver[3] == 0) {
                line_read();
                linetrace(70);
        }
        LED(1,0,0,0);
        MD.motor(0,0); delay(200);
}
/****************************************Miningpit****************************************/
INLINE void miningpit(){
        while(analogRead(CN4) < 500) {
                line_read();
                linetrace(40);
                //MD.motor(62,49);
        }
        MD.motor(0,0);
        delay(100);
        while(analogRead(CN4) >= 500) {
                MD.motor(-50,-50);
        }
        #ifdef Daisen
        delay(120);//微調整
        #else
        delay(100);//微調整
        #endif
        MD.motor(0,0);
        delay(100);

        #ifdef Daisen

        MD.motor(70,-1); delay(300);//変更10:25


        while(line.OnSilver[2] == 0) {//変更ない
                line_update();
                MD.motor(70,-1);//60,0
        }

        MD.motor(-200,0);
        delay(100);
        while(line.OnSilver[4] == 0) {
                line_update();
                MD.motor(0,-70);
        }
        #else
        while(line.OnSilver[2] == 0 &&line.OnSilver[1] == 0&&line.OnSilver[0] == 0) {//変更ない
                line_update();
                MD.motor(70,-1);//60,0
        }
        MD.motor(-70,0); delay(100);
        MD.motor(0,0);
        delay(100);
        while(line.OnSilver[4] == 0 &&line.OnSilver[5] == 0 && line.OnSilver[6] == 0) {
                line_update();
                MD.motor(0,-70);
        }
        #endif
        MD.motor(0,0);
        delay(100);
        setflont(0);
        Timer.reset();
        myservo.write(180,55);
        while(Timer.read_ms() < 500) {
                Spin_to_flont(-55,-55);
        }
        MD.motor(0,0);
        Timer.reset();
        while(Timer.read_ms() < 400) {
                Spin_to_flont(105,100);
        }
        MD.motor(0,0);
        myservo.write(90,100);
        delay(500);
        MD.motor(-100,100);
        delay(250);
        MD.motor(0,0);
        delay(50);
        #ifdef Daisen
        MD.motor(150,150);
        delay(130);
        #else
        MD.motor(150,150);
        delay(200);
        #endif
        while(analogRead(line.channel[3]) > White_Threshold3) {
                MD.motor(100,100);
        }
        LED(1,0,0,0);
        MD.motor(100,0);
        delay(380);
        LED(0,0,0,0);
}
/****************************************ムーンベース１で置くプログラム****************************************/
INLINE void moonbase1(){
        Buzzer(0);
        MD.motor(0,0);
        delay(100);

        #ifdef Daisen
        while(line.OnSilver[4] == 0) {
                line_update();
                MD.motor(0,70);
        }
        myservo.write(180,90);
        delay(120);
        MD.motor(0,0); delay(100);
        MD.motor(-100,-100);
        delay(200);

                #else
        myservo.write(180,90);
        while(line.OnSilver[4] == 0 &&line.OnSilver[5] == 0 && line.OnSilver[6] == 0) {
                line_update();
                MD.motor(0,70);
        }

                #endif
        //delay(100);
        MD.motor(0,0);
        delay(200);
        MD.motor(-100,-100);
        delay(300);
        while(analogRead(line.channel[3]) > White_Threshold3) {
                MD.motor(-200,-200);
        }
        delay(50);
        MD.motor(0,0);
        delay(100);
        MD.motor(200,0);
        delay(220);
        Timer.reset();
        while(Timer.read_ms() < 500) {
                line_read();
                linetrace(80);
        }
        myservo.write(90);

        #ifdef Daisen
        while(line.OnSilver[2] == 0) {
                line_read();
                linetrace(90);
        }

        #else
        while(line.OnSilver[0] == 0&&line.OnSilver[1] == 0&&line.OnSilver[2] == 0) {
                line_read();
                linetrace(90);
        }
        #endif
}
/****************************************ムーンベース２で置くプログラム****************************************/
INLINE void moonbase2(){
        MD.motor(-100,-100);
        delay(100);
        MD.motor(70,0);//60,0
        delay(500);

        #ifdef Daisen
        while(line.OnSilver[2] == 0 ) {//変更ない
                line_update();
                MD.motor(80,0);//60,0
        }
        MD.motor(0,0);
        delay(100);
        while(line.OnSilver[4]) {
                line_update();
                MD.motor(0,-80);
        }
        #else

        while(line.OnSilver[2] == 0 &&line.OnSilver[1] == 0&&line.OnSilver[0] == 0) {//変更ない
                line_update();
                MD.motor(80,0);//60,0
        }
        MD.motor(0,0);
        delay(100);

        while(line.OnSilver[4] == 0 &&line.OnSilver[5] == 0 && line.OnSilver[6] == 0) {
                line_update();
                MD.motor(0,-80);
        }
        #endif
        myservo.write(180,90);
        MD.motor(0,0);
        delay(400);
        #ifdef Daisen
        MD.motor(100,-100);///角度調整
        delay(100);
        #else
        //MD.motor(-100,100);///角度調整
        //delay(100);
        #endif
        MD.motor(0,0);
        delay(100);
        #ifdef Daisen
        MD.motor(-100,-100);
        delay(260);
        /*while(analogRead(line.channel[3]) < White_Threshold3) {
                MD.motor(-100,-100);
           }*/
        #else
        MD.motor(-100,-100);
        delay(100);
        #endif
        MD.motor(0,0);
        delay(100);
        while(analogRead(line.channel[3]) > White_Threshold3) {
                MD.motor(-100,-100);
        }
        delay(120);
        MD.motor(0,0);
        delay(100);
        MD.motor(0,200);
        delay(200);//henkou
        Timer.reset();
        while(Timer.read_ms() < 100) {
                line_read();
                linetrace(70);
        }
        myservo.write(90);
}
/****************************************ムーンベース３で置くプログラム****************************************/
INLINE void moonbase3(){
        MD.motor(80,-80); delay(260);//旋回
        Buzzer(0);
        MD.motor(100,100); delay(200);
        myservo.write(180,90);
        MD.motor(0,0); delay(1000);//停止
        //MD.motor(80,80); delay(500);//前進
        MD.motor(-100,-100); delay(300);//後退
        line_update();
        while(line.OnBlack[3] == 0) {
                line_update();
                MD.motor(-100,-100);
        }
        MD.motor(0,0); delay(300);//停止
        //MD.motor(80,-80); delay(300);//旋回
        line_update();
        while(line.OnBlack[2] == 0) {
                line_update();
                MD.motor(100,0);
        }





        //MD.motor(0,0); while(1) ;
        line_update();
        while(line.OnSilver[3] == 0) {
                line_read();
                linetrace(90);
        }
        MD.motor(0,0); delay(200);
        MD.motor(100,0); delay(400);//旋回
        MD.motor(70,80); delay(300);
        MD.motor(0,0); LED(1,1,0,0); Buzzer(3); while(1) ;

}

INLINE void avoid_container(){
        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        sensors.US = USread(0);
        Timer.reset();
        while(sensors.US > 20) {
                line_read();
                linetrace(50);
                sensors.US = USread(0);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }
        /*  while(sensors.US < 18) {
                  //line_read();
                  //linetrace(50);
                  MD.motor(-100,-100);
                  sensors.US = USread(0);
                  if(Timer.read_ms() > 2000) {
                          break;
                  }
           }*/
        MD.motor(0,0); delay(100);
        MD.motor(0,100); delay(200);
        MD.motor(40,40); delay(300);
        MD.motor(90,0); delay(200);
        MD.motor(40,40); delay(460);
        MD.motor(90,0); delay(200);
        MD.motor(40,40); delay(400);
        line_update();
        while(line.OnWhite[3]) {
                line_update();
                MD.motor(40,40);//140,40
        }
        delay(200);
        MD.motor(-80,80); delay(200);
        MD.motor(-200,-200); delay(100);
/*
        int goalcm = 10;
        int movespeeds = 70;
        int minbackspeed = -50;
        float gain = 2;
        int m;
        myservo.write(80);

        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        sensors.US = USread(0);
        Timer.reset();
        while(sensors.US > 20) {
                line_read();
                linetrace(50);
                sensors.US = USread(0);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }
        MD.motor(0,0); delay(200);
        line_update();
 #ifdef Daisen
        while(line.OnWhite[4]) {
                line_update();
                MD.motor(0,60);
        }
        while(line.OnWhite[4] == 0) {
                line_update();
                MD.motor(0,60);
        }
        MD.motor(70,0); delay(700);//追加しました
 #else
        while(line.OnWhite[6]) {
                line_update();
                MD.motor(0,60);
        }
        while(line.OnWhite[6] == 0) {
                line_update();
                MD.motor(0,60);
        }
 #endif
        MD.motor(0,0); delay(100);
        Buzzer(0);
        MD.motor(80,70); delay(300);
        sensors.US2 = USread(1);
        while(sensors.US2 < 15) {
                sensors.US2 = USread(1);
                MD.motor(70,70);
        } Buzzer(0); delay(300);
        MD.motor(100,0); delay(330);//300
        //MD.motor(150,70); delay(400);
        Buzzer(0);
        MD.motor(80,60); delay(400);
        while(line.OnWhite[3]) {
                line_update();
                MD.motor(120,40);//140,40
        }
        MD.motor(0,0);
        delay(200);
        MD.motor(-200,200);
        delay(150);
        MD.motor(0,0);
        delay(100);
        line_read();*/
/*

        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        sensors.US = USread(0);
        Timer.reset();
        while(sensors.US > 20) {
                line_read();
                linetrace(50);
                sensors.US = USread(0);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }

        setflont(90);
        MD.motor(0,0); delay(700);
        Spin_to_flont2(80);
        MD.motor(0,0); delay(500);
        sensors.US2 = USread(1);
        Timer.reset();
        while(sensors.US2 < 20) {
                sensors.US2 = USread(1);
                MD.motor(50,50);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }

        MD.motor(0,0);
        delay(400);
        setflont(-90);
        Spin_to_flont2(80);
        MD.motor(0,0); delay(500);
        while(1) ;


 *//*


        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        sensors.US = USread(0);
        Timer.reset();
        while(sensors.US > 15) {
                line_read();
                linetrace(50);
                sensors.US = USread(0);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }
        LED(1,1,1,1);
        MD.motor(0,0);
        delay(200);
        while(line.OnWhite[0]) {
                line_update();
                MD.motor(0,70);
        }
        ///delay(50);
        MD.motor(100,100);
        delay(220);
        MD.motor(100,0);
        delay(100);
        MD.motor(100,100);
        delay(100);
        MD.motor(1000,0);
        delay(150);
        while(line.OnWhite[3]) {
                line_update();
                MD.motor(70,70);
        }
 *//*
        Timer.reset();
        while(Timer.read_ms() < 300) {
                line_read();
                linetrace(50);
        }
        sensors.US = USread(0);
        Timer.reset();
        while(sensors.US > 20) {
                line_read();
                linetrace(50);
                sensors.US = USread(0);
                if(Timer.read_ms() > 2000) {
                        break;
                }
        }

        while(line.OnWhite[6]) {
                line_update();
                MD.motor(70,0);
        }
        delay(100);



        while(line.OnWhite[3]) {
                line_update();
                sensors.US2 = USread(1);
                if(sensors.US2 < goalcm) {
                        m = (goalcm - sensors.US2)*gain;
                        m = movespeeds - m;
                        if(m < minbackspeed) {
                                m = minbackspeed;
                        }
                        MD.motor(movespeeds,m);
                }else if(sensors.US2 > goalcm) {
                        m = (sensors.US2 - goalcm)*gain;
                        m = movespeeds - m;
                        if(m < minbackspeed) {
                                m = minbackspeed;
                        }
                        MD.motor(m,movespeeds);
                }
                //delay(10);
        }
        MD.motor(100,-100); delay(200);  /*//*

                                            MD.motor(0,0);
                                            delay(100);
                                            sensors.US2 = USread(1);
                                            while(sensors.US2 > 30) {
                                            sensors.US2 = USread(1);
                                            Spin_to_flont(-70,-70);
                                            }
                                            while(sensors.US2 < 30) {
                                            sensors.US2 = USread(1);
                                            Spin_to_flont(100,100);
                                            }
                                            delay(300);
                                            MD.motor(0,0);
                                            delay(500);
                                            setflont(-90);
                                            Timer.reset();
                                            while(Timer.read_ms() < 1000) {
                                            Spin_to_flont(0,0);
                                            }
                                            sensors.US2 = USread(1);
                                            while(sensors.US2 < 30) {
                                            sensors.US2 = USread(1);
                                            Spin_to_flont(50,50);
                                            }
                                            delay(300);
                                            setflont(-80);
                                            Timer.reset();
                                            while(Timer.read_ms() < 1000) {
                                            Spin_to_flont(0,0);
                                            }
                                            while(analogRead(line.channel[3]) > White_Threshold3) {
                                            MD.motor(70,70);
                                            }

                                            //*/
}
