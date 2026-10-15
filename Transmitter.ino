#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>
#include <math.h>

int stick1xPin=A2;
int stick1yPin=A3;
int stick2xPin=A4;
int stick2yPin=A5;

int testPin = 8;

int trim1Pin=2;
int trim2Pin=3;
int trim3Pin=4;
int trim4Pin=5;

int trimInPin = A0;
int throttleTrimInPin = A1;

uint8_t trim1;
uint8_t trim2;
uint8_t trim3;
uint8_t trim4;

bool rollTrimOn = false;
bool pitchTrimOn = false;
bool yawTrimOn = false;
bool throttleTrimOn = false;

uint8_t trim1Prev=1;
uint8_t trim2Prev=1;
uint8_t trim3Prev=1;
uint8_t trim4Prev=1;

int motorSwitchPin=6;
uint8_t motorSwitch;
uint8_t motorSwitchPrev=1;
uint8_t motorLive=0;

float stick1x;
float stick1y;
float stick2x;
float stick2y;

float trimIn;
float throttleTrimIn;
float maxTrim = 50.;
float maxThrottleTrim = 50.;

float rollTrim = 0;
float pitchTrim = 0;
float yawTrim = 0;
float throttleTrim = 0;

RF24 radio(9,7); //instantiating the radio object and setting CE to pin 9 and CSN to pin 7 (CSN pin 8 when testing with UNO)
const byte address[10] = "ADDRESS01";
uint8_t package[5];

void setup() {
  // put your setup code here, to run once:
  delay(2000); // lets the capacitors charge up before handshake attempt (doesn't appear to be necessary for nano every, is necessary for uno)
  Serial.begin(9600);

  pinMode(stick1xPin,INPUT);
  pinMode(stick1yPin,INPUT);
  pinMode(stick2xPin,INPUT);
  pinMode(stick2yPin,INPUT);
  pinMode(trim1Pin,INPUT_PULLUP);
  pinMode(trim2Pin,INPUT_PULLUP);
  pinMode(trim3Pin,INPUT_PULLUP);
  pinMode(trim4Pin,INPUT_PULLUP);
  pinMode(trimInPin,INPUT);
  pinMode(throttleTrimInPin,INPUT); 
  pinMode(motorSwitchPin,INPUT_PULLUP);
  pinMode(testPin,OUTPUT);
  // digitalWrite(testPin, HIGH);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
};

void loop() {
  // put your main code here, to run repeatedly:

  //gets all inputs
  stick1x = analogRead(stick1xPin)*(255./1023.);
  stick1y = analogRead(stick1yPin)*(255./1023.);
  stick2x = analogRead(stick2xPin)*(255./1023.);
  stick2y = analogRead(stick2yPin)*(255./1023.);

  trim1 = digitalRead(trim1Pin);
  trim2 = digitalRead(trim2Pin);
  trim3 = digitalRead(trim3Pin);
  trim4 = digitalRead(trim4Pin);

  trimIn = analogRead(trimInPin)*(maxTrim/1023.);
  throttleTrimIn = analogRead(throttleTrimInPin)*(maxThrottleTrim/1023.);

  motorSwitch = digitalRead(motorSwitchPin);

  //turns trim bools on and off
  if(trim1 == 0 && trim1Prev == 1){
    rollTrimOn = !rollTrimOn;
    trim1Prev = 0;
  }else if(trim1 == 1){
    trim1Prev = 1;
  }

  if(trim2 == 0 && trim2Prev == 1){
    pitchTrimOn = !pitchTrimOn;
    trim2Prev = 0;
  }else if(trim2 == 1){
    trim2Prev = 1;
  }

  if(trim3 == 0 && trim3Prev == 1){
    yawTrimOn = !yawTrimOn;
    trim3Prev = 0;
  }else if(trim3 == 1){
    trim3Prev = 1;
  }

  if(trim4 == 0 && trim4Prev == 1){
    throttleTrimOn = !throttleTrimOn;
    trim4Prev = 0;
  }else if(trim4 == 1){
    trim4Prev = 1;
  }

  //updates active trims
  if (rollTrimOn) {
    rollTrim = trimIn;
  }
  if (pitchTrimOn) {
    pitchTrim = trimIn;
  }
  if (yawTrimOn) {
    yawTrim = trimIn;
  }
  if (throttleTrimOn) {
    throttleTrim = throttleTrimIn;
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
  if((stick1x+rollTrim)>255){
    stick1x = 255-rollTrim;
  }else if((stick1x+rollTrim)<0){
    stick1x = 0-rollTrim;
  }

  if((stick1y+pitchTrim)>255){
    stick1y = 255-pitchTrim;
  }else if((stick1y+pitchTrim)<0){
    stick1y = 0-pitchTrim;
  }

  if((stick2x+yawTrim)>255){
    stick2x = 255-yawTrim;
  }else if((stick2x+yawTrim)<0){
    stick2x = 0-yawTrim;
  }

  if((stick2y+throttleTrim)>255){
    stick2y = 255-throttleTrim;
  }else if((stick2y+throttleTrim)<0){
    stick2y = 0-throttleTrim;
  }

  //sends stick1x, stick1y, stick2x, stick2y, and motorSwitch values to reciever
  // package[0] = (uint8_t)round(stick1x);
  package[0] = 5;
  package[1] = (uint8_t)round(stick1y);
  package[2] = (uint8_t)round(stick2x);
  package[3] = (uint8_t)round(stick2y);
  package[4] = motorLive;
  
  radio.write(&package,sizeof(package));
  
};