/*  nowRailV1_7_1_HandController4_0
    05/01/2026

   This tab contains custom functions that are called when nowrail receives commands.
   This allows users to write their own code driven by these events using the varibles passed.

   If not required all these functions can be commented out of the tab deleted.

*/

//Accessory Command Received.. this function allows you to process NON built in commands
//or add extra custom features
//Accnum and accInst received. Custom fetures, animations can be called from this function
//Function can be deleted
void nowAccComRec(int accNum, byte accInst) {
  myLayout.accProcessed(1);  //processed... required for panel update transmission

  Serial.print("Accessory Command for AccNum:");
  Serial.print(accNum);
  Serial.print(" Dir:");
  Serial.println(accInst);

  switch (accNum) {

    case 70:  //accessory number 70
      if (accInst > 0) {
        //      Serial.println("Acc 70 moved 1");
      } else {
        //      Serial.println("Acc 70 moved 0");
      }
      break;

    default:
      myLayout.accProcessed(0);  //wasn't processed by this board so no panel update
      break;
  }
}

//control panel update command received after an Accessory command has been completed
//The accNUm and accInst are received and can be used for custom control panel functions
//Function can be deleted
void nowPanelUpdate(int accNum, byte accInst) {
  // Serial.print("Panel Update for AccNum:");
  // Serial.print(accNum);//The accessory number that has been processed
  // Serial.print(" Dir:");
  // Serial.println(accInst); //direction/instruction/direction completed

  switch (accNum) {
    /*
    case 25: //accessory number 25
      if(accInst > 0){
          digitalWrite(ledLeftPin,HIGH);//control panel leds
          digitalWrite(ledRightPin,LOW);//control panel leds
      }else{
          digitalWrite(ledLeftPin,LOW);
          digitalWrite(ledRightPin,HIGH);
      }
      break;
    
      
    default:
      break;
      */
  }
}


//This function is called every layout second update (fast clock that can mean a lot of times per real world second )
//This function can be used to update layout clock either on controllers or layout scenic displays/clocks
//It can also be used to send commands at set times, this may control lights to come on/off at certain times in a building
//Function can be deleted
void nowTimeEvents(byte clockSpeed, byte clockHour, byte clockMinute, byte clockSecond, byte clockDay) {
  // Serial.print("nowTimeEvents ");
  // Serial.print("clockSpeed: ");
  // Serial.print(clockSpeed);
  // Serial.print(" Time: ");
  // Serial.print(clockHour);
  // Serial.print(" : ");
  // Serial.print(clockMinute);
  // Serial.print(" : ");
  // Serial.print(clockSecond);
  // Serial.print(" Day: ");
  // Serial.print(clockDay);
  // Serial.println(" ");
   screenDay(clockDay);//screen day update
  screenTime(clockHour,clockMinute,clockSecond);
}

//receives loco function updates..for updating controllers
void nowLocoFuncUpdate(int locoAddr, byte nowFuncNum, byte nowFuncState){
  //update controller or loco?
  // Serial.println("nowLocoFuncUpdate: ");
  // Serial.println(locoAddr);
  // Serial.println(nowFuncNum);
  // Serial.println(nowFuncState);
  screen1LocoFunctions();
}

//receives speed updates
void nowLocoSpeedUpdate(int locoAddr, byte locoSpeed, byte locoDir){
  //update controller or loco?
  // Serial.println("nowLocoSpeedUpdate: ");
  // Serial.println(locoAddr);
  // Serial.println(locoSpeed);
  // Serial.println(locoDir);

}

//function called when clock speed change received
void nowClockSpeedUpdate(){
  //update controller
  screenTimeAcc();
}

//CUSTOM Function
//this function allows users to trigger events based on times. It is not a built in function but shows how custom functions can be added.
//This is called from main loop and just calls various rtcClock...events to demonstrate some built in functions.
//Function can be deleted
/*
void myTimeEvents() {
  byte myHour;
  byte myMinute;
  byte mySeconds;
  byte myDay;
  byte myClockSpeed;

  // if (currentMillis - timeMillis >= timeTimer) {
  //   timeMillis = currentMillis;

    // myClockSpeed = myLayout.rtcClockSpeed();
    // Serial.print("Clock Speed: ");
    // Serial.print(myClockSpeed);

    // myHour = myLayout.rtcHours();
    // Serial.print(" Hours: ");
    // Serial.print(myHour);

    // myMinute = myLayout.rtcMinutes();
    // Serial.print(" Minutes: ");
    // Serial.print(myMinute);

    // mySeconds = myLayout.rtcSeconds();
    // Serial.print(" Seconds: ");
    // Serial.print(mySeconds);

    // myDay = myLayout.rtcDays();
    // Serial.print(" Day: ");
    // Serial.println(myDay);

  
  // }
}
*/
//triggered by GT911 touch screen touches
//Allows custom functions to be written from GT911 screen presses
void nowGT911Touch(int xPos, int yPos) {
  Serial.println("custom function nowGT911Touch");
  Serial.print("xPos: ");
  Serial.print(xPos);
  Serial.print(" yPos: ");
  Serial.println(yPos);
}

//This function gets called every loop
//It allows you to check other buttons/sensors not built in
//Any button inherits nowRails debounce code
void nowMomentButton(void) {
  /*
  Any custom button or sensor code can be placed in this function
  It is called everytime the void loop() runs.
  Code should control your own custom functions
  */

  
}

//Receives power commands that would usually be sent to DCC-EX by a line like myLayout.sendPowerCommand(DONEEMERGENCYSTOP);
//Function would be used by loco controllers to update power staus if needed.
void nowPowerCommand(byte Command) {
  Serial.print("nowPowerCommand: ");
  Serial.println(Command);
  //OPTIONS
  // #define TURNPOWEROFF 0...Turn power off
  // #define TURNPOWERON 1
  // #define TURNEMERGENCYSTOP 2
  // #define DONEPOWEROFF 3...power turned off
  // #define DONEPOWERON 4.
  // #define DONEEMERGENCYSTOP 5
}

//Outputs sensor updates
void nowSensorUpdate(int senNum, byte senInst) {
  //Uncomment to see sensor traffic
  Serial.print("sensor: ");
  Serial.print(senNum);
  Serial.print(" : ");
  Serial.println(senInst);
}

//function called when bulk upload received and processed
void nowLocoBulkDataRX(){
  rxAllState = 0;
  buildscreen7();//reset button on settings screen
}

//function called when single loco data has been received
void nowLocoDataSetRX(){
  
  //hand controller because this function gets called when loco data has been received
  //If this controller is waiting for data
  locoRXbuttonState = 0;
  buildscreen3();//rebuild screen as buttons and text will have probably changed
   
  //All data from transmission stored in byte locoTXdataSet[32]
  // data0 ... unused reserved
  // data1 ... unused reserved
  // data2 ... DCC address HIgh Byte
  // data3 ... DCC address Low Byte
  // data4 ... functions 0-7
  // data5 ... functions 8-15
  // data6 ... functions 16-23
  // data7 ... functions 24-31
  // data8 ... functions 32-39
  // data9 ... functions 40 - 47
  // data10 ... functions 48 - 55
  // data11 ... functions 56 - 63...allows for 64 functions for future proofing
  // data12 ...  consist state (0-3)
  // data13 - 31 Loco Name ascii chars
  
  //This will print out the whole received data set
  int q;
  Serial.println("nowLocoDataSetRX:");
  for(q=0;q<32;q++){
    Serial.println(myLayout.getLocoTXdataSetByte(q));
  }

}

