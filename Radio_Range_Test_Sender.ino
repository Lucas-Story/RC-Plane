#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

int button = 0;
bool startup = false;
bool recieved = false;

RF24 radio(9,7); //instantiating the radio object and setting CE to pin 9 and CSN to pin 10 (CSN pin 8 when testing with UNO)
const byte address[5] = {'f','t','h','i','s'};
char txt0[2] = "0";
char txt1[2] = "1";

void setup() {
  // put your setup code here, to run once:
  delay(2000); // lets the capacitors charge up before handshake attempt (doesn't appear to be necessary for nano every, is necessary for uno)
  pinMode(4,INPUT_PULLUP);
  pinMode(3,OUTPUT);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening();
  digitalWrite(3,LOW);
  if (radio.isChipConnected()){
    startup = true;
  }
};

void loop() {
  // put your main code here, to run repeatedly:
  button = digitalRead(4);
  if (button==1){
    recieved = radio.write(&txt1,sizeof(txt1));
  }else{
    recieved = radio.write(&txt0,sizeof(txt0));
  }

  if (recieved){
    digitalWrite(3,HIGH);
  }else{
    digitalWrite(3,LOW);
  }

  delay(200);
};