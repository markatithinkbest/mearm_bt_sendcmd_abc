/*
 * 10/19/2015
 * Mark Chen in Caotun, Taiwan
 *
 */
#include <SoftwareSerial.h>
#include <Servo.h>

// === 調試使用 (由於伺服馬達的起始位置不完全相同,可供不同 MeArm 使用)===

//int STD_DELAY=12; //連續動作時,每一度的delay, 4很快,12很有機器人的慢節奏
int LOOP_DELAY=15; //
int MOVE_DELAY=50; //一個小動作後的delay
//int LOOP_STEP=1;

int degA=30;
int degB=90;
int degC=150;

//int mode=0;


//int D90=90;

// for fixed combo
int m1Min=60; //閉
int m1Max=100; //開
int m2Min=75; // 後
int m2Max=125; // 前
int m3Min=45;  // 下
int m3Max=100; // 上
int m4Min=30; // m4右 順時針
int m4Max=150; // m4左 逆時針


// for manual 
//int m1Min=60; //閉
//int m1Max=100; //開
//int m2Min=30; // 後
//int m2Max=150; // 前
//int m3Min=30;  // 下
//int m3Max=150; // 上
//int m4Min=30; // m4右 順時針
//int m4Max=150; // m4左 逆時針



// === 程式底層定義 (共同,不要隨意更動)===
String str;
SoftwareSerial BT(12, 13);

// 4個伺服馬達
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;



// 四個pin腳
// Arduino Senser Sheld 5.0
int m1Pin=3;
int m2Pin=5;
int m3Pin=6;
int m4Pin=9;

int m1Pos=90;
int m2Pos=90;
int m3Pos=90;
int m4Pos=90;

void servoRun(int servoNum,int pos){
  Servo servo;
  int fromPos;
  
  if (servoNum==1){
    servo=servo1;
    fromPos=m1Pos;
    if (pos>m1Max || pos<m1Min){
      return;
    }
    m1Pos=pos;
  }

  if (servoNum==2){
    servo=servo2;
    fromPos=m2Pos;
    if (pos>m2Max || pos<m2Min){
      return;
    }
    m2Pos=pos;
  }
  
  if (servoNum==3){
    servo=servo3;
    fromPos=m3Pos;
    if (pos>m3Max || pos<m3Min){
      return;
    }
    m3Pos=pos;
  }
  
  if (servoNum==4){
    servo=servo4;
    fromPos=m4Pos;
    if (pos>m4Max || pos<m4Min){
      return;
    }
    m4Pos=pos;
  }  

  //
  servoAct(servo, fromPos, pos);
}


// 給定的伺服馬達,從 m 運動到 n
// 這兩個值的區間是 0 到180
// 但受到手臂結構限制,不能全部運動
// 因此要一個一個調試
void servoAct(Servo servo,int m, int n){
  if (m<n){
    for (int i=m;i<=n;i++){
      servo.write(i*1);
        delay(LOOP_DELAY);
        
    }
  }  
  if (m>n){
    for (int i=m;i>=n;i--){
      servo.write(i*1);
        delay(LOOP_DELAY);
    }
  } 
  delay(MOVE_DELAY);
 
}


void takeObjLite(){
   servoRun(1,m1Max);//
  servoRun(3,m3Min);//手臂向下  
  servoRun(2,m2Max);//手臂向前
  delay(500); 
  //m1Close(); 
  servoRun(1,m1Min);//
  delay(500); 
  servoRun(2,m2Min); 
  servoRun(3,m3Max);  
}
void putObjLite(){
  servoRun(3,m3Min);//手臂向下  
  servoRun(2,m2Max);//手臂向前
  delay(500); 
  //m1Close(); 
  servoRun(1,m1Max);//
  delay(500); 
  servoRun(2,m2Min); 
  servoRun(3,m3Max);  
}

void initPos(){
  servoRun(1,90);
  servoRun(2,90);
  servoRun(3,90);
  servoRun(4,90);
  delay(1000);
}

void moveObjFromAtoB(int fromPos, int toPos){
  servoRun(4,fromPos);  
  takeObjLite();  
  servoRun(4,toPos);
  putObjLite();
}

void setup() {
  // === attach ===
  servo1.attach(m1Pin); // 手爪
  servo2.attach(m2Pin); // 前後
  servo3.attach(m3Pin); // 上下
  servo4.attach(m4Pin); // 底盤

  // === init Serial and Bluetooth ===
  Serial.begin(9600);
  BT.begin(9600);
}

void runCmd(String str){
    if (str=="=>=>"){
      initPos();
      return;
    }     
    if (str=="A=>A"){
      moveObjFromAtoB(degA,degA);
      return;    
    }
    if (str=="A=>B"){
      moveObjFromAtoB(degA,degB);
      return;    
    }
    if (str=="A=>C"){
      moveObjFromAtoB(degA,degC);
      return;    
    }
    if (str=="B=>A"){
      moveObjFromAtoB(degB,degA);
      return;    
    }
    if (str=="B=>B"){
      moveObjFromAtoB(degB,degB);
      return;    
    }
    if (str=="B=>C"){
      moveObjFromAtoB(degB,degC);
      return;    
    }
    if (str=="C=>A"){
      moveObjFromAtoB(degC,degA);
      return;    
    }
    if (str=="C=>B"){
      moveObjFromAtoB(degC,degB);
      return;    
    }
    if (str=="C=>C"){
      moveObjFromAtoB(degC,degC);
      return;    
    }
    if (str=="DEMO1"){
      moveObjFromAtoB(degA,degB);
      moveObjFromAtoB(degC,degA);
      moveObjFromAtoB(degB,degC);
      initPos();
      return;    
    }
}

void loop() {
  String cmd;
  if (Serial.available()>0){
    cmd=Serial.readString();
    runCmd(cmd); 
  }
  if (BT.available()>0){
    cmd=BT.readString();
    runCmd(cmd);      
  }
}


