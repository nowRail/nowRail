const int encButton = 25;
const int encLeft = 26;
const int encRight = 27;

uint8_t lrmem = 3;
int lrsum = 0;

//encode button pressed
void encButtonPress(){
  if(currentMillis - screenMillis >= screenTimer){
    
    if(digitalRead(encButton) < 1){//pull up resistor
      screenMillis = currentMillis;
      if(locoSpeed < 128){
        locoSpeed = 127;
      }else{
        locoSpeed = 128;
      }
      screenLocoSpeed();//update the screen 
    }
  }
}

void encUpdateScreen(){//any updates due to encoder changes
  if(locoSpeedFlag > 0){
    //Serial.println(locoSpeed);
    screenLocoSpeed();
    locoSpeedFlag = 0;
  }
}

void rotorInterrupt() {
  int8_t res;
  res = rotary();
  if (res != 0)
  {
    
        if(locoSpeed > 0 && res < 0){//keep loco speed within 0-255
          locoSpeed = locoSpeed + res;
          locoSpeedFlag = 1;
        }
        if(locoSpeed < 255 && res > 0){//keep loco speed within 0-255
          locoSpeed = locoSpeed + res;
          locoSpeedFlag = 1;
        }
        //Serial.println(locoSpeed);
    
    //encNum = encNum + res;
    //Serial.println(encNum );
  }
}

//function that works out what the endoder is doing.
int8_t rotary()
{
  static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};
  int8_t l, r;

  //l = digitalRead(encLeft);
 // r = digitalRead(encRight);
 //Swapped over as new encoder was running backwards
  r = digitalRead(encLeft);
  l = digitalRead(encRight);

  lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;
  lrsum = lrsum + TRANS[lrmem];
  /* encoder not in the neutral state */
  if (lrsum % 4 != 0) return (0);
  /* encoder in the neutral state */
  if (lrsum == 4)
  {
    lrsum = 0;
    return (1);
  }
  if (lrsum == -4)
  {
    lrsum = 0;
    return (-1);
  }
  /* lrsum > 0 if the impossible transition */
  lrsum = 0;
  return (0);
}

void encoderSetUp(){
  pinMode(encButton, INPUT_PULLUP);//Internal pull up for encoder button
  pinMode(encLeft, INPUT);
  pinMode(encRight, INPUT);
  attachInterrupt(encLeft, rotorInterrupt, CHANGE);
  attachInterrupt(encRight, rotorInterrupt, CHANGE);

}