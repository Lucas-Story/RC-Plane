#include <Servo.h>

Servo aileronRight;
Servo aileronLeft;
Servo elevator;
Servo rudder;

int aileronRightPin = 11;
int aileronLeftPin = 6;
int elevatorPin = 5;
int rudderPin = 3;

int rollPin = A0;
int pitchPin = A1;
int yawPin = A2;

int rollIn;
int pitchIn;
int yawIn;

int rollOut;
int pitchOut;
int yawOut;

int rollTrimPin = A3;
int pitchTrimPin = A4;
int yawTrimPin = A5;

int rollTrim;
int pitchTrim;
int yawTrim;

void setup() {
  // put your setup code here, to run once:

  //servo setup
  pinMode(aileronRightPin,OUTPUT);
  pinMode(aileronLeftPin,OUTPUT);
  pinMode(elevatorPin,OUTPUT);
  pinMode(rudderPin,OUTPUT);

  aileronRight.attach(aileronRightPin,500,2500);
  aileronLeft.attach(aileronLeftPin,500,2500);
  elevator.attach(elevatorPin,500,2500);
  rudder.attach(rudderPin,500,2500);

  //joystick setup
  pinMode(rollPin,INPUT);
  pinMode(pitchPin,INPUT);
  pinMode(yawPin,INPUT);

  //trim knob setup
  pinMode(rollTrimPin,INPUT);
  pinMode(pitchTrimPin,INPUT);
  pinMode(yawTrimPin,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //gets trim inputs and scales them to fit into trim range (full trim is 50% of full ROM in either direction, so max trim is +/- 23 degrees)
  rollTrim = (analogRead(rollTrimPin)*(46./1023.))-23.;
  pitchTrim = (analogRead(rollTrimPin)*(46./1023.))-23.;
  yawTrim = (analogRead(rollTrimPin)*(46./1023.))-23.;

  //gets joystick inputs
  rollIn = analogRead(rollPin);
  pitchIn = analogRead(pitchPin);
  yawIn = analogRead(yawPin);
  
  //rounds joystick inputs, accounting for deadzone
  if(rollIn>=500 && rollIn<=520){
    rollIn = 512;
  }
  if(pitchIn>=500 && pitchIn<=520){
    pitchIn = 512;
  }
  if(yawIn>=500 && yawIn<=520){
    yawIn = 512;
  }

  //turns joystick postition into a servo position
  rollOut = (rollIn*(90./1023.)) + 45 + rollTrim;
  pitchOut = (pitchIn*(90./1023.)) + 45 + pitchTrim;
  yawOut = (yawIn*(90./1023.) + 45) + yawTrim;
  
  //ensures no position is out of range (prevents out of range error from degrees of rotation added from trim)
  if(rollOut>135){
    rollOut = 135;
  }
  if(rollOut<45){
    rollOut = 45;
  }
  if(pitchOut>135){
    pitchOut = 135;
  }
  if(pitchOut<45){
    pitchOut = 45;
  }
  if(yawOut>135){
    yawOut = 135;
  }
  if(yawOut<45){
    yawOut = 45;
  }

  //assigns positions to their respective servos
  aileronRight.write(rollOut);
  aileronLeft.write(180-rollOut);
  elevator.write(pitchOut);
  rudder.write(yawOut);

}