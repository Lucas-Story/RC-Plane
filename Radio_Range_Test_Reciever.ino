#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>


RF24 radio(9,8); //instantiating the radio object and setting CE to pin 9 and CSN to pin 10
const byte address[5] = {'f','t','h','i','s'};
char txt[2] = "1";
bool startup = false;


void setup() {
  // put your setup code here, to run once:
  delay(2000); // lets the capacitors charge up before handshake attempt (doesn't appear to be necessary for nano every, is necessary for uno)
  pinMode(4,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS); // lower data rate to prevent ringing at higher voltage logic levels on SPI pins
  radio.openReadingPipe(1, address);
  radio.startListening();
  if (radio.isChipConnected()){
    startup = true;
  }
  digitalWrite(4,LOW);
  radio.flush_rx();
};

void loop() {

  // put your main code here, to run repeatedly:
  if (radio.available()){
    txt[0] = '0';
  
    radio.read(&txt, sizeof(txt));

    if(txt[0] == '1'){
      digitalWrite(4,HIGH);
    }else{
      digitalWrite(4,LOW);
    }
  }
  Serial.println(txt);

};