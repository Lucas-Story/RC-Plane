//upload to Arduino and then unplug
//set up a simple push button
//plug esc signal wire into signal input of esc
//power Arduino with same battery as esc using a splitter and the VIN and GND pins
//plug in battery to splitter
//wait until starts short beeping and then press the button (holding not necessary, but don't just tap it)
//wait for two short beeps then 2 long beeps
//unplug battery
//Successful Calibration!

#include <Servo.h>

int escPin = 9;
int buttonPin = 5;
int button=1;
Servo esc;

void setup() {
  // put your setup code here, to run once:
  pinMode(escPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  esc.attach(escPin,1000,2000);
  esc.writeMicroseconds(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(button == 1){
    button = digitalRead(buttonPin);
    delay(100);
  }
  esc.writeMicroseconds(1000);
}
