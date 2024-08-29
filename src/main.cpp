#include <Arduino.h>
#include <ESP32Servo.h> 
#include <PS4Controller.h>
#include "PS4Controller.h"
#include "CytronMotorDriver.h"
#include <IcsHardSerialClass.h>
/* ここにサーボモータのライブラリをインクルードする */
//
//

// PS4コントローラーのMACアドレス
// 使いたいPS4コントローラのMACアドレスをあらかじめ調べておく
#define PS4_ADDR "64:82:14:12:57:57" 
//64:82:14:12:57:57

void ReceiveControllerInput();

float l_x = 0.0; // 左スティックのX軸
float l_y = 0.0; // 左スティックのY軸
float r_x = 0.0; // 右スティックのX軸
float r_y = 0.0; // 右スティックのY軸


// Configure the motor driver.
//GPIO 0,1,3,14,15,34,35,36,39 は使用しない
CytronMD rfmotor(PWM_DIR, 18, 19);  // PWM = Pin 18, DIR = Pin 19.
CytronMD lfmotor(PWM_DIR, 32, 33);
CytronMD rbmotor(PWM_DIR, 16, 17);
CytronMD lbmotor(PWM_DIR, 25, 26);

const int a=18;
const int b=19;
const int c=32;
const int d=33;
const int e=25;
const int f=26;
const int g=16;
const int h=17;

//bool stopOrMove=false;//停止かそれ以外か false=stop,true=move
  bool rotate=false;//その場で回転するか false=nomalMove,true=rotate
  bool m1=false;
  bool m2=false;
  bool m3=false;

// ボタンの状態を保存する配列(単押し, 長押し判定用)
bool prev_bttn_state[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

void setup() {
  Serial.begin(115200);
  PS4.begin(PS4_ADDR);
  Serial.println("> PS4: Started");
  Serial.println("> PS4: Press PS button to connect");
  // PS4コントローラーが接続されるまで待つ
  while (!PS4.isConnected()) {
    Serial.println("> PS4: Connecting...");
    delay(1000);
  }
  // 接続されたら，LEDを白色にして振動させる
  Serial.println("> PS4: Connected");
  PS4.setLed(255, 255, 255);
  PS4.setRumble(200, 200);
  PS4.sendToController(); // PS4に送信(これでLEDが白色＆振動する)
  delay(1000); // 1秒間待つ
  PS4.setRumble(0, 0);
  PS4.sendToController(); // PS4に送信(これで振動が止まる)

  //モーターピンの初期設定
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(h,OUTPUT);

  /* ここにサーボモータの初期化コードを書く */
  //
  //
}

void loop() {
  ReceiveControllerInput(); // PS4コントローラーの入力を受け取る
  Serial.printf("> PS4: LStickX: %f, LStickY: %f, RStickX: %f, RStickY: %f\n", l_x, l_y, r_x, r_y);
  /* ここにスティックの値に応じたサーボモータの制御コードを書く */

  //255 128

  if(-0.2<l_x&&l_x<0.2&&-0.2<l_y&&l_y<0.2){
    //停止
    rfmotor.setSpeed(0);
    lfmotor.setSpeed(0);
    rbmotor.setSpeed(0);
    lbmotor.setSpeed(0);
    //rotate=true;
    Serial.println("stop");
  }
  else if(-0.2<=l_x&&l_x<=0.2&&0.2<=l_y&&l_y<=1.1){
    //前
    rfmotor.setSpeed(-255);
    lfmotor.setSpeed(255);
    rbmotor.setSpeed(-255);
    lbmotor.setSpeed(255);
    rotate=false;
    Serial.println("front");
  }
  else if(0.2<=l_x&&l_x<=0.2&&-1.1<=l_y&&l_y<=-0.2){
    //後ろ
    rfmotor.setSpeed(255);
    lfmotor.setSpeed(-255);
    rbmotor.setSpeed(255);
    lbmotor.setSpeed(-255);
    rotate=false;
    Serial.println("back");
  }
  else if(0.2<=l_x&&l_x<=1.1&&-0.2<=l_y&&l_y<=0.2){
    //右
    rfmotor.setSpeed(255);
    lfmotor.setSpeed(255);
    rbmotor.setSpeed(-255);
    lbmotor.setSpeed(-255);
    rotate=false;
    Serial.println("right");
  }
  else if(-1.1<=l_x&&l_x<=-0.2&&-0.2<=l_y&&l_y<=0.2){
    //左
    rfmotor.setSpeed(-255);
    lfmotor.setSpeed(-255);
    rbmotor.setSpeed(255);
    lbmotor.setSpeed(255);
    rotate=false;
    Serial.println("left");
  }
  else if(0.2<=l_x&&l_x<=1.1&&0.2<=l_y&&l_y<=1.1){
    //斜め右前
    rfmotor.setSpeed(0);
    lfmotor.setSpeed(255);
    rbmotor.setSpeed(-255);
    lbmotor.setSpeed(0);
    rotate=false;
    Serial.println("right front");
  }
  else if(-1.1<=l_x&&l_x<=-0.2&&0.2<=l_y&&l_y<=1.1){
    //斜め左前
    rfmotor.setSpeed(-255);
    lfmotor.setSpeed(0);
    rbmotor.setSpeed(0);
    lbmotor.setSpeed(255);
    rotate=false;
    Serial.println("left front");
  }
  else if(0.2<=l_x&&l_x<=1.1&&-1.1<=l_y&&l_y<=-0.2){
    //斜め右後ろ
    rfmotor.setSpeed(255);
    lfmotor.setSpeed(0);
    rbmotor.setSpeed(0);
    lbmotor.setSpeed(-255);
    rotate=false;
    Serial.println("right back");
  }
  else if(-1.1<=l_x&&l_x<=-0.2&&-1.1<=l_y&&l_y<=-0.2){
    //斜め左後ろ
    rfmotor.setSpeed(0);
    lfmotor.setSpeed(-255);
    rbmotor.setSpeed(255);
    lbmotor.setSpeed(0);
    rotate=false;
    Serial.println("left back");
  }
  if(rotate){
    if(0.2<=r_x&&r_x<=1.1){
      //右旋回
      rfmotor.setSpeed(255);
      lfmotor.setSpeed(255);
      rbmotor.setSpeed(255);
      lbmotor.setSpeed(255);
      Serial.println("right rotate");
    }
    else if(-1.1<=r_x&&r_x<-0.2){
      //左旋回
      rfmotor.setSpeed(-255);
      lfmotor.setSpeed(-255);
      rbmotor.setSpeed(-255);
      lbmotor.setSpeed(-255);
      Serial.println("left rotate");
    }
  }
  delay(100); // delayの秒数[ms]は適宜変更する. 10ms = 0.01s.
}

// PS4コントローラーの入力を受け取る
void ReceiveControllerInput() {
  if (PS4.LatestPacket() && PS4.isConnected()) {
    // スティックの値を取得して，値の範囲を-1.0 ~ 1.0に正規化する
    l_x = PS4.LStickX() / 127.0; // -127 ~ 127 -> -1.0 ~ 1.0
    l_y = PS4.LStickY() / 127.0; // -127 ~ 127 -> -1.0 ~ 1.0
    r_x = PS4.RStickX() / 127.0; // -127 ~ 127 -> -1.0 ~ 1.0
    r_y = PS4.RStickY() / 127.0; // -127 ~ 127 -> -1.0 ~ 1.0

    // ボタン押したときの挙動をifの中に書く
    if (PS4.Up() && !prev_bttn_state[0]) {
      Serial.println("> PS4: Up button pressed");
    } 
    if (PS4.Down() && !prev_bttn_state[1]) {
      Serial.println("> PS4: Down button pressed");
    }
    if (PS4.Left() && !prev_bttn_state[2]) {
      Serial.println("> PS4: Left button pressed");
    }
    if (PS4.Right() && !prev_bttn_state[3]) {
      Serial.println("> PS4: Right button pressed");
    }
    if (PS4.Triangle() && !prev_bttn_state[4]) {
      Serial.println("> PS4: Triangle button pressed");
    }
    if (PS4.Circle() && !prev_bttn_state[5]) {
      Serial.println("> PS4: Circle button pressed");
    }
    if (PS4.Cross() && !prev_bttn_state[6]) {
      Serial.println("> PS4: Cross button pressed");
    }
    if (PS4.Square() && !prev_bttn_state[7]) {
      Serial.println("> PS4: Square button pressed");
    }
    if (PS4.L1() && !prev_bttn_state[8]) {
      rotate=false;
      m1=false;
      m2=true;
      m3=false;
      Serial.println("> PS4: L1 button pressed");
    }
    if (PS4.R1() && !prev_bttn_state[9]) {
      rotate=false;
      m1=true;
      m2=false;
      m3=false;
      Serial.println("> PS4: R1 button pressed");
    }
    if (PS4.L2() && !prev_bttn_state[10]) {
      rotate=false;
      m1=false;
      m2=false;
      m3=true;
      Serial.println("> PS4: L2 button pressed");
    }
    if (PS4.R2() && !prev_bttn_state[11]) {
      rotate=true;
      m1=false;
      m2=false;
      m3=false;
      Serial.println("> PS4: R2 button pressed");
    }
    if (PS4.Share() && !prev_bttn_state[12]) {
      Serial.println("> PS4: Share button pressed");
    }
    if (PS4.Options() && !prev_bttn_state[13]) {
      Serial.println("> PS4: Options button pressed");
    }
    if (PS4.L3() && !prev_bttn_state[14]) {
      Serial.println("> PS4: L3 button pressed");
    }
    if (PS4.R3() && !prev_bttn_state[15]) {
      Serial.println("> PS4: R3 button pressed");
    }

    // デフォルトだと，押した瞬間だけ判定される．
    // 長押し判定するなら，任意のボタンの部分をコメントアウトする↓
    /* 
      UPボタンを長押しできるようにしたいとき 
      例: // prev_bttn_state[0] = PS4.Up(); 
    */
    prev_bttn_state[0] = PS4.Up();
    prev_bttn_state[1] = PS4.Down();
    prev_bttn_state[2] = PS4.Left();
    prev_bttn_state[3] = PS4.Right();
    prev_bttn_state[4] = PS4.Triangle();
    prev_bttn_state[5] = PS4.Circle();
    prev_bttn_state[6] = PS4.Cross();
    prev_bttn_state[7] = PS4.Square();
    prev_bttn_state[8] = PS4.L1();
    prev_bttn_state[9] = PS4.R1();
    prev_bttn_state[10] = PS4.L2();
    prev_bttn_state[11] = PS4.R2();
    prev_bttn_state[12] = PS4.Share();
    prev_bttn_state[13] = PS4.Options();
    prev_bttn_state[14] = PS4.L3();
    prev_bttn_state[15] = PS4.R3();
  }
}