#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <printf.h>

RF24 radio(9,8); //CE,CSN (CSN is 7 for nano every and 8 for uno)
byte addresses[][6] = {"1Node", "2Node"};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();
  printf_begin();
  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  /*const char txt[] = "Hellow World";
  radio.write(&txt, sizeof(txt));
  Serial.println("Hellow World");
  delay(1000);
  */
}
