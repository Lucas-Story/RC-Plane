#include <Servo.h>

int stick1xPin=A2;
int stick1yPin=A3;
int stick2xPin=A1;
int stick2yPin=A0;

int servo1Pin=5;
int servo2Pin=6;
int servo3Pin=3;
int servo4Pin=10;

int escPin=9;

int trim1Pin=2;
int trim2Pin=4;
int trim3Pin=7;
int trim4Pin=8;
int trim5Pin=12;
int trim6Pin=13;

int trim1;
int trim2;
int trim3;
int trim4;
int trim5;
int trim6;

int trim1Prev=1;
int trim2Prev=1;
int trim3Prev=1;
int trim4Prev=1;
int trim5Prev=1;
int trim6Prev=1;

int motorSwitchPin=11;
int motorSwitch;
int motorSwitchPrev=1;
int motorLive=0;

int stick1x;
int stick1y;
int stick2x;
int stick2y;

int rollTrim;
int pitchTrim;
int yawTrim;



Servo aileronLeft;
Servo aileronRight;
Servo rudder;
Servo elevator;

Servo ESC;


void setup() {
  // put your setup code here, to run once:
  pinMode(stick1xPin,INPUT);
  pinMode(stick1yPin,INPUT);
  pinMode(stick2xPin,INPUT);
  pinMode(stick2yPin,INPUT);

  pinMode(servo1Pin,OUTPUT);
  pinMode(servo2Pin,OUTPUT);
  pinMode(servo3Pin,OUTPUT);
  pinMode(servo4Pin,OUTPUT);

  pinMode(escPin,OUTPUT);

  pinMode(trim1Pin,INPUT);
  pinMode(trim2Pin,INPUT);
  pinMode(trim3Pin,INPUT);
  pinMode(trim4Pin,INPUT);
  pinMode(trim5Pin,INPUT);
  pinMode(trim6Pin,INPUT);

  pinMode(motorSwitchPin,INPUT);

  aileronLeft.attach(servo1Pin,500,2500);
  aileronRight.attach(servo2Pin,500,2500);
  elevator.attach(servo4Pin,500,2500);
  rudder.attach(servo3Pin,500,2500);  

  aileronLeft.write(90);
  aileronRight.write(90);
  elevator.write(90);
  rudder.write(90);

  ESC.attach(escPin,1000,2000);
  ESC.writeMicroseconds(1000);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:

  //gets all inputs
  stick1x = 45.+(analogRead(stick1xPin)*(90./1023.));
  stick1y = 45.+(analogRead(stick1yPin)*(90./1023.));
  stick2x = 45.+(analogRead(stick2xPin)*(90./1023.));
  stick2y = 1000.+(analogRead(stick2yPin)*(1000./1023.));

  trim1 = digitalRead(trim1Pin);
  trim2 = digitalRead(trim2Pin);
  trim3 = digitalRead(trim3Pin);
  trim4 = digitalRead(trim4Pin);
  trim5 = digitalRead(trim5Pin);
  trim6 = digitalRead(trim6Pin);

  motorSwitch = digitalRead(motorSwitchPin);

  //sets trim values
  if(trim1 == 0 && trim1Prev == 1){
    rollTrim = rollTrim + 1;
    trim1Prev = 0;
  }else if(trim1 == 1){
    trim1Prev = 1;
  }

  if(trim2 == 0 && trim2Prev == 1){
    rollTrim = rollTrim - 1;
    trim2Prev = 0;
  }else if(trim2 == 1){
    trim2Prev = 1;
  }

  if(trim3 == 0 && trim3Prev == 1){
    pitchTrim = pitchTrim + 1;
    trim3Prev = 0;
  }else if(trim3 == 1){
    trim3Prev = 1;
  }

  if(trim4 == 0 && trim4Prev == 1){
    pitchTrim = pitchTrim - 1;
    trim4Prev = 0;
  }else if(trim4 == 1){
    trim4Prev = 1;
  }

  if(trim5 == 0 && trim5Prev == 1){
    yawTrim = yawTrim + 1;
    trim5Prev = 0;
  }else if(trim5 == 1){
    trim5Prev = 1;
  }

  if(trim6 == 0 && trim6Prev == 1){
    yawTrim = yawTrim - 1;
    trim6Prev = 0;
  }else if(trim6 == 1){
    trim6Prev = 1;
  }

  if(rollTrim>25){
    rollTrim = 25;
  }else if(rollTrim<-25){
    rollTrim = -25;
  }

    if(pitchTrim>25){
    pitchTrim = 25;
  }else if(pitchTrim<-25){
    pitchTrim = -25;
  }

    if(yawTrim>25){
    yawTrim = 25;
  }else if(yawTrim<-25){
    yawTrim = -25;
  }

  //sets motor kill switch value
  if(motorSwitch == 0 && motorSwitchPrev == 1){
    if(motorLive == 0){
      motorLive = 1;
    }else{
      motorLive = 0;
    }
    motorSwitchPrev = 0;
  }else if(motorSwitch == 1){
    motorSwitchPrev = 1;
  }

  //ensures trims don't force any inputs out of range
  if((stick1x+rollTrim)>135){
    stick1x = 135-rollTrim;
  }else if((stick1x+rollTrim)<45){
    stick1x = 45-rollTrim;
  }

  if((stick1y+pitchTrim)>135){
    stick1y = 135-pitchTrim;
  }else if((stick1y+pitchTrim)<45){
    stick1y = 45-pitchTrim;
  }

  if((stick2x+yawTrim)>135){
    stick2x = 135-yawTrim;
  }else if((stick2x+yawTrim)<45){
    stick2x = 45-yawTrim;
  }

  //outputs
  aileronLeft.write(180-(stick1x+rollTrim));
  aileronRight.write(stick1x+rollTrim);
  elevator.write(stick1y+pitchTrim);
  rudder.write(stick2x+yawTrim);

  if(motorLive == 1){
    ESC.writeMicroseconds(3000-stick2y);
  }else{
    ESC.writeMicroseconds(1000);
  }

}
