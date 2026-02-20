/*nowRailV1_9_3
13/02/2026

1.9.3
PCA9685LEDOPENDRAIN PCA9685 open drain mode and inverts signal so all functions/effects remain the same
1.9.2
Consists have seperate Clear and Set allowing consists to remain selected in list but not active
Press button when stopped to change direction..working mods in encButtonPress() in encoder.ino
Configuration for how control knob works... configured by setting #define CLOCKWISESPPEDINCREASE 1 in controller_setup.h
loco controller encode interrupt routine improved... less chance of crash/panic

1.9.1
detach PCA9685 servo after movement

PCA9685 flashing, reverse flashing can be done by reversing the min and max 
mods
void addPCA9685Led(byte board, byte port, int accNum, int dirOn, int effect, int maxBright, int effectBright);  //dir)n = 0 or 1 to turn on, effect 0 = on/0ff, 1 = fire flicker, 2 = gas light, 3 = arc welder, 4 = flash

#define PCA9685FLASHTIMER 1000 //any flashing panel or accessory Leds flash timing in milliseconds

MIT License
Copyright (c) 2026 Simon Coward
Copyright (c) 2026 Digital Town

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


//NON Nowrail variables
unsigned long currentMillis = millis();


void setup() {
  Serial.begin(115200);//Standard Serial Output to Serial monitor
  //Serial.begin(115200, SERIAL_8N2);//SERIAL_8N2 required if JMRICMRI connection
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));  //File details

  //Start the system
  myLayout.init();//This functions sets up ESP-NOW as well as other items needed for the system to run
   
}

void loop() {
  currentMillis = millis();
  myLayout.runLayout();//This sits in the main loop and needs to run as often as possible
   
}
