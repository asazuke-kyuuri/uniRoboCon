#include <Arduino.h>
#include<ESP32Servo.h>


Servo myservo;  // サーボモータオブジェクトを作成

int servoPin = 18;  // サーボモータの信号ピン（GPIO 18）

void setup() {
  pinMode(servoPin, OUTPUT);  // サーボモータの信号ピンを出力モードに設定
  myservo.attach(servoPin);  // サーボモータを指定したピンに接続
}

void loop() {


  myservo.write(180);
 
}