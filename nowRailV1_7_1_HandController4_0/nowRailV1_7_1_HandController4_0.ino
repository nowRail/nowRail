/*nowRailV1_7_1_HandController4_0
06/01/2026

    Adds Layout set clock time
    bug fix
    consists cleared when new loco recalled or selected

 MIT License
Copyright (c) 2025 Simon Coward
Copyright (c) 2025 Digital Town

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 

*/
//This line sets up the system
#include "nowRail.h"  //include the nowRail class
//This is your layout unique ID code. Can stay as is but change if you will be using near other nowRail layouts.
nowRail myLayout(0x00, 0x01, 0x02, 0x03); 

#include "TFT_eSPI.h"
#include "FT6236.h"
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library with default width and height
FT6236 ts = FT6236();

//Fonts
#include "Free_Fonts.h" // Include the header file attached to this sketch

//NON Nowrail variables
unsigned long currentMillis = millis();

unsigned long screenMillis;//screen press timer...also used for encoder debounce

int screenTimer = 250;//screen debounce

byte locoID;//Current Loco under control
byte locoSpeed = 128; //loco speed
byte locoSpeedFlag = 0;//flag from encoder


int funcSet;//group of functions
byte powerState;//power state command#include "TFT_eSPI.h"
byte locoRXbuttonState;//0 = normal state, 1 = waiting for transmission 
byte rxAllState;

void setup() {
  Serial.begin(115200);//Standard Serial Output to Serial monitor
  //Serial.begin(115200, SERIAL_8N2);//SERIAL_8N2 required if JMRICMRI connection
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));  //File details

  
  //Start the system
  myLayout.init();//This functions sets up ESP-NOW as well as other items needed for the system to run
  screenSetUp();//sets up LCD screen
  touchSetUp();//sets up touch 
  encoderSetUp();//set up encoder
  //myLayout.sendClockSpeedChange(1);//set clock speed to 1 for testing
}

void loop() {
  currentMillis = millis();
  myLayout.runLayout();//This sits in the main loop and needs to run as often as possible
  touchProcess(); //calls the touch process and recall timer
  encUpdateScreen();//any updates due to encoder changes
  encButtonPress();
  //custom function
  //myTimeEvents();  //sample function that deals with time events
}
