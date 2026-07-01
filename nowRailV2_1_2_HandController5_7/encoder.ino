/*nowRailV2_1_2_HandControllerV5_7
18/06/2026
*/
const int encButton = 25;
const int encLeft = 26;
const int encRight = 27;
volatile int8_t res;
uint8_t lrmem = 3;
int lrsum = 0;

//encode button pressed
//v5 mods
void encButtonPress() {
  if (currentMillis - screenMillis >= SCREENBUTTONDEBOUNCE) {
    if (digitalRead(encButton) < 1) {  //pull up resistor
      screenMillis = currentMillis;
      //if the train is moving stop it
      if (locoSpeed < 127 || locoSpeed > 128) {
        if (locoSpeed < 128) {
          locoSpeed = 127;
        } else {
          locoSpeed = 128;
        }
      } else {  //if loco is already stopped
        if (locoSpeed == 127) {
          locoSpeed = 128;
        } else {
          locoSpeed = 127;
        }
        if(DIRECTIONORPOWER > 0 && screenMode == 1){//5.6 mod button being used for direction control, must only happen on screen mode 1
          screen1OtherButtons();  //redraw buttons
        }
      }
      screenLocoSpeed();  //update the screen
    }
  }
}





void encUpdateScreen() {  //any updates due to encoder changes
  if (locoSpeedFlag > 0) {
    screenLocoSpeed();
    locoSpeedFlag = 0;
  }
}

//v5 mods start here... definitions for clockwise only speed increase
void rotorInterrupt() {
  int8_t res;
  res = rotary();
  if (res != 0) {   
#if defined(CLOCKWISESPEEDINCREASE)
    //new version only allows forward or reverse speed changes
    if (locoSpeed > 127) {  //forwards
      if (res > 0 && locoSpeed < 255) {
        locoSpeed++;
        locoSpeedFlag = 1;
      } else if (res < 0 && locoSpeed > 128) {
        locoSpeed--;
        locoSpeedFlag = 1;
      }
    }
    //reverse
    if (locoSpeed < 128) {  //reverse
      if (res > 0 && locoSpeed > 0) {
        locoSpeed--;
        if (locoSpeed > 127) {
          locoSpeed = 127;
        }
        locoSpeedFlag = 1;
      } else if (res < 0 && locoSpeed < 127) {

        locoSpeed++;
        locoSpeedFlag = 1;
      }
    }
#else
    //my original version allows to rotate until reverse
    if (locoSpeed > 0 && res < 0) {  //keep loco speed within 0-255
      locoSpeed--;
      locoSpeedFlag = 1;
    }
    if (locoSpeed < 255 && res > 0) {  //keep loco speed within 0-255
      locoSpeed++;
      locoSpeedFlag = 1;
    }

#endif
  }
}

//function that works out what the endoder is doing.
int8_t rotary() {
  static int8_t TRANS[] = { 0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0 };
  int8_t l, r;

//v5 mod to reverse encoder direction
#if defined(REVENCODER)  //reverse order if defined
  l = digitalRead(encLeft);
  r = digitalRead(encRight);
#else
  r = digitalRead(encLeft);
  l = digitalRead(encRight);
#endif

  lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;
  lrsum = lrsum + TRANS[lrmem];
  /* encoder not in the neutral state */
  if (lrsum % 4 != 0) return (0);
  /* encoder in the neutral state */
  if (lrsum == 4) {
    lrsum = 0;
    return (1);
  }
  if (lrsum == -4) {
    lrsum = 0;
    return (-1);
  }
  /* lrsum > 0 if the impossible transition */
  lrsum = 0;
  return (0);
}

void encoderSetUp() {
  pinMode(encButton, INPUT_PULLUP);  //Internal pull up for encoder button
  pinMode(encLeft, INPUT);
  pinMode(encRight, INPUT);
  attachInterrupt(encLeft, rotorInterrupt, CHANGE);
  attachInterrupt(encRight, rotorInterrupt, CHANGE);
}