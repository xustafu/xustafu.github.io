#ifndef fantasia_h_
#define fantasia_h_

// use: https://github.com/dgduncan/SevenSegment
#include <SegmentDisplay.h>


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>

//Teensy audio initialization Code
//SD CARD PINS
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13

#define BUTTON_UP 0
#define BUTTON_DOWN 1

class Fantasia
{
public:

  //Display LEDs Declaration
  //                                  E   D   C  DP  B   A   G   F
  //                                  1   2   4  5   6   7   9  10
  SegmentDisplay segmentDisplay;//  (31, 28, 33, 9, 32, 30, 26, 29);

  //Encoder declaration, button in buttons section
  Encoder enc;

  //initialization list
  Fantasia(): enc(0,1), segmentDisplay(31, 28, 33, 9, 32, 30, 26, 29) {

  }


  // select the input pins for the potentiometers
  int potPin1 = A0; 
  int potPin2 = A1; 
  int potPin3 = A2; 
  int potPin4 = A3; 
  int potPin5 = A8;
  

  // pushbutton connected to digital pinS
  int btnPin1 = 2;
  int btnPin2 = 5; 
  int btnPin3 = 25; 
  int btnPin4 = 27; 
  int gatePin1 = 3; 
  int gatePin2 = 6; 
  int encBtnPin = 4;

  // Variables to store interface values 
  float pot1 = 0; 
  float pot2 = 0;
  float pot3 = 0;
  float pot4 = 0;
  float pot5 = 0;

  int btn1 = BUTTON_UP;
  int btn2 = BUTTON_UP;
  int btn3 = BUTTON_UP;
  int btn4 = BUTTON_UP;

  int gate1 = 0;
  int gate2 = 0;

  int encPressed = 0; // is encoder button pressed

  //methods
  void setup();
  void update();

  int sdCardSetup();

private:
  int maxSdReadAttempts = 10;
  int numSdReadAttempts = 0;

  void pinSetup();
       
};

#endif