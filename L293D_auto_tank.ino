#include <AFMotor.h>
#include<Servo.h>
 
Servo servo1; //任意のサーボモータークラスを用意。

AF_DCMotor right_motor(4);
AF_DCMotor left_motor(1);

const int trig = 15; // 出力ピン
const int echo = 14; // 入力ピン
const int ir = 4; // 入力ピン
const int servopwm = 9; // Servolib only 9 or 10

void setup() {
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  servo1.attach(servopwm); //デジタル9番ピンをサーボモーターの出力ピンとして設定

  // turn on motor
  right_motor.setSpeed(200);
  left_motor.setSpeed(200);
 
  right_motor.run(RELEASE);
  left_motor.run(RELEASE);
}

void loop() {

  ircheck();
  right_check();
  ircheck();
  forward_check();
  ircheck();
  left_check();
  ircheck();
  forward_check();
  
  moter_forward(90,110,200);
  
}

float measure(){
  
  // 超音波の出力終了
  digitalWrite(trig,LOW);
  delayMicroseconds(1);
  // 超音波を出力
  digitalWrite(trig,HIGH);
  delayMicroseconds(11);
  // 超音波を出力終了
  digitalWrite(trig,LOW);
  // 出力した超音波が返って来る時間を計測
  int t = pulseIn(echo,HIGH);
  // 計測した時間と音速から反射物までの距離を計算
  float distance = t*0.017;
  // 計算結果をシリアル通信で出力
  Serial.print(distance);
  Serial.println(" cm");
  //delay(500);

  if (distance < 0){
    distance = 20;
  }

  return distance;
}

void ircheck(){
  int irresult;
  irresult = analogRead(ir)  ;   // IRセンサーから読込む
  Serial.print(irresult) ;    // シリアルモニターに表示させる
  Serial.println(" irresult");
  if (irresult < 100){
      avoid();
  }
}


void moter_forward(int L_duty, int R_duty, int keeptime){

  right_motor.setSpeed(R_duty);
  left_motor.setSpeed(L_duty);
  right_motor.run(FORWARD);
  left_motor.run(FORWARD);
  delay(keeptime);

}

void moter_back(int L_duty, int R_duty, int keeptime){
  
  right_motor.setSpeed(R_duty);
  left_motor.setSpeed(L_duty);
  right_motor.run(BACKWARD);
  left_motor.run(BACKWARD);
  delay(keeptime);

}

void moter_left(int L_duty, int R_duty, int keeptime){
  
  right_motor.setSpeed(R_duty);
  left_motor.setSpeed(L_duty);
  right_motor.run(FORWARD);
  left_motor.run(BACKWARD);
  delay(keeptime);

}

void moter_right(int L_duty, int R_duty, int keeptime){
  
  right_motor.setSpeed(R_duty);
  left_motor.setSpeed(L_duty);
  right_motor.run(BACKWARD);
  left_motor.run(FORWARD);
  delay(keeptime);

}

void moter_stop(int keeptime){
  
  right_motor.run(RELEASE);
  left_motor.run(RELEASE);
  delay(keeptime);
    
}


void right_check(){

  servo1.write(45); //right
  delay(300);
  
  float r_cm = measure();
    
  if(r_cm < 19){
      moter_stop(10);
      moter_left(50,60,200);
  }
}


void forward_check(){
  
  servo1.write(90); //forward
  delay(300);

  float distance = measure();
  
  if (distance < 13){
      avoid();
  }
}

void left_check(){
  
  servo1.write(135); //left
  delay(300);
  
  float l_cm = measure(); 
    
  if(l_cm < 19){
     moter_stop(10);
     moter_right(50,60,200);
  }
}

void avoid(){
 
  moter_stop(100);
  moter_back(60,70,1000);
  moter_stop(100);
  servo1.write(0); //right
  delay(500);
  float r_cm = measure();
  servo1.write(180); //left
  delay(500);
  float l_cm = measure(); //forward
  servo1.write(90); //forward
  delay(300);

  if(r_cm > l_cm)  {
      moter_right(60,70,1000);
  }
  else{
      moter_left(60,70,1000);
  }
    
}
