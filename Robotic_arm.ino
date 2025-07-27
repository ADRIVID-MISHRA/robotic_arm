#include "Wire.h"
#include <MPU6050_light.h>
#include <Servo.h>

#define potPin A0
int servoPin1 = 11;
Servo servo1;
//int angle1 = 0;  //servo for elbow: roll .. set initaial angle

int servoPin2= 10;
Servo servo2;
int angle2=90;

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");

  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

  
  pinMode(potPin,INPUT);

  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  servo1.attach(servoPin1);

  servo2.attach(servoPin2);
  // servo2.write(angle2);
  // delay(50);

}

void loop() {
  mpu.update();
  
  int roll=mpu.getAngleX();
  int pitch=mpu.getAngleY();
  int yaw=mpu.getAngleZ();

	Serial.print("X : ");
	Serial.print(roll);
	Serial.print("\tY : ");
	Serial.print(pitch);
	Serial.print("\tZ : ");
	Serial.println(yaw);

  //roll control: elbow servo
  if(roll>30){
    servo1.write(100);
  }
  if(roll<-30 && roll>-45){
    servo1.write(85);
  }
  if(roll<-45){
    servo1.write(50);
  }
  if(roll<30 && roll>-30){
    servo1.write(90);
  }
  
  //pitch control: accuator dc motor
  if(pitch>30){
    analogWrite(enB, 255);
    digitalWrite(in3, LOW);
	  digitalWrite(in4, HIGH);
  }
  if(pitch<-30){
    analogWrite(enB, 255);
    digitalWrite(in3, HIGH);
	  digitalWrite(in4, LOW);
  }
  if(pitch<30 && pitch>-30){
    analogWrite(enB, 0);
    digitalWrite(in3, LOW);
	  digitalWrite(in4, LOW);
  }
  
  //yaw control: base dc motor
  if(yaw>30){
    analogWrite(enA, 255);
    digitalWrite(in1, HIGH);
	  digitalWrite(in2, LOW);
  }
  if(yaw<-30){
    analogWrite(enA, 255);
    digitalWrite(in1, LOW);
	  digitalWrite(in2, HIGH);
  }
  if(yaw<30 && yaw>-30){
    analogWrite(enA, 0);
    digitalWrite(in1, LOW);
	  digitalWrite(in2, LOW);
  }
  
  int potReading= analogRead(potPin);
  //angle2=map(potReading,300,800,0,70);
  if(potReading>550){
    angle2=70;
  }
  if(potReading<550){
    angle2=0;
  }
  
  servo2.write(angle2);
  
  Serial.println(potReading);
  Serial.println(angle2);
  delay(200);
}