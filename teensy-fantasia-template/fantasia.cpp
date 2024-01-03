#include "Fantasia.h"

void Fantasia::setup(){
  pinSetup();

}

void Fantasia::update(){

  pot1 = analogRead(potPin1);
  pot2 = analogRead(potPin2);
  pot3 = analogRead(potPin3);
  pot4 = analogRead(potPin4);
  pot5 = analogRead(potPin5);
  btn1 = !digitalRead(btnPin1);
  btn2 = !digitalRead(btnPin2);
  btn3 = !digitalRead(btnPin3);
  btn4 = !digitalRead(btnPin4);
  encPressed = !digitalRead(encBtnPin);
  

}


void Fantasia::pinSetup() {
  // sets the digital pins as inputs and set pullups
  //Serial.println("Setting digital pins...");
  pinMode(btnPin1, INPUT_PULLUP);
  pinMode(btnPin2, INPUT_PULLUP);
  pinMode(btnPin3, INPUT_PULLUP);
  pinMode(btnPin4, INPUT_PULLUP);
  pinMode(gatePin1, INPUT_PULLUP);
  pinMode(gatePin2, INPUT_PULLUP);
  pinMode(encBtnPin, INPUT_PULLUP);
  //Serial.println("done");

  // set Display LEDs ports as Outputs
  //Serial.println("Setting display...");
  pinMode(30, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(9, OUTPUT);
  //Serial.println("done");
}


int Fantasia::sdCardSetup() {
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (numSdReadAttempts < maxSdReadAttempts) {
      // Serial.println("Unable to access the SD card");
      numSdReadAttempts++;
      delay(500);
    }
    return 0;
  } else {
    return 1;
  }
  numSdReadAttempts = 0;
}