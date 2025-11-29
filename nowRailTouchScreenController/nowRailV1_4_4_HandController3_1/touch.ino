int doublePress = 200;
unsigned long doublePressTimer;//extra check for same box pressed twice
int debouncePeriod = 500;//debounce sensitivity
unsigned long debounceTimer;
int lastBox;
byte clockSpeed;
int q;
int sendStaticAccNum;//accessory num to be sent

//processes each sceen box
void touchProcessBox(int box){
  
    byte funcNum;
    byte funcState;
    int myConstState;
    byte  sendDir;
    int sendSpeed;
    int q;
    // Serial.print("b: ");
    // Serial.println(box);
    if(currentMillis - debounceTimer >= debouncePeriod){
        debounceTimer = currentMillis;
    switch(box){
        case 3 ... 4://loco name number speed
            break;
        case 5 ... 14://functions
            funcNum = box - 5 + funcSet;
            funcState =  myLayout.getLocoDCCFuncState(locoID, funcNum);
            if(funcState > 0){
              funcState = 0;
            }else{
              funcState = 1;
            }
            myLayout.setLocoDCCFuncState(locoID, funcNum, funcState);
            break;
        case 15: // << Func
            if(funcSet > 0){
                funcSet = funcSet - 10;
                screen1LocoFunctions();//redraw function boxes
            }
            break;
        case 16:
            if(funcSet < 20){
                funcSet = funcSet + 10;
                screen1LocoFunctions();//redraw function boxes
            }
            break;
        case 17: //change loco
            buildscreen2();
            break;
        case 18://Recall
            locoRecallPressed();
            break;
        case 19://consist
            buildscreen6();//consist screen
            break;
        case 20://Settings
            buildscreen7();
            break;
        case 21://STOP/Start
            if (locoSpeed < 128) {  //Reverse
              sendDir = 0;
              sendSpeed = map(locoSpeed, 0, 127, 127, 0);
            } else {  //forward
              sendDir = 1;
              sendSpeed = map(locoSpeed, 128, 255, 0, 127);
            }
           
            if(powerState < 1){//turn off
                powerState = 1;
                
                myLayout.sendPowerCommand(powerState,myLayout.getLocoDCCAddress(locoID), sendSpeed,sendDir);
            }else{ //turn on
                powerState = 0;
                myLayout.sendPowerCommand(powerState,myLayout.getLocoDCCAddress(locoID), sendSpeed, sendDir);   
            }
             Serial.print("PWR:");
            Serial.println(powerState);
            screen1OtherButtons();//redraw buttons
            break;
        
        //SCREEN 2
        case 23:
            buildscreen1();
            break;
         case 24://loco select
             buildscreen3();
             break;
        case 25 ... 32://loco select buttons
            locoSelectSelectedLoco = box - 25 + locoSelectScrollOffset;
            locoSelectBoxes();
            break;
        case 33://scroll up
            if(locoSelectScrollOffset > 8){
                locoSelectScrollOffset = locoSelectScrollOffset - 8;
            }else{
                locoSelectScrollOffset = 0;
            }
            locoSelectBoxes();
            break;
        case 34://scroll down
            if(locoSelectScrollOffset < 200 - 16){
                locoSelectScrollOffset = locoSelectScrollOffset + 8;
            }else{
                locoSelectScrollOffset = 200 - 9;
            }
            //Serial.println(locoSelectScrollOffset);
            locoSelectBoxes();
            break;
        
        //Screen 3
        case 35://back to loco select
            buildscreen2();
            break;
        case 36://Loco Selected
            setNewCurrentLoco();
            // myLayout.locoReCallDataUpdate(locoID,locoSpeed);//Update the recall system...current loco
            // //stop current loco
            // locoSpeed = 128;//set speed to 0 forwards
            // screenLocoSpeed();
            // //now set the new loco
            
            // locoID = locoSelectSelectedLoco;
            // myLayout.locoReCallDataUpdate(locoID,locoSpeed);//Update the recall system...new loco
            // screenLocoNameNum();
            // screenLocoSpeed();
            // buildscreen1();
            break;
        case 37: //Receive << 
            break;
        case 38: //Send 
            break;
        case 39: //Edit Loco Num
            buildscreen4();//edit loco number 
            break;
        case 40: //Edit Loco name
             buildscreen5();//edit loco number 
             break;
        case 41: //exit number change 
            buildscreen3();
            break;
        case 42: //Update number change 
            myLayout.setLocoDCCAddress(locoSelectSelectedLoco, editLocoNum);
            buildscreen3();
            break;
        case 44:
            if(editLocoNum < 9000){
                editLocoNum = editLocoNum + 1000;
                editLocoNumBox();
            }
            break;
        case 45:
            if(editLocoNum < 9900){
                editLocoNum = editLocoNum + 100;
                editLocoNumBox();
            }
            break;
        case 46:
            if(editLocoNum < 9990){
                editLocoNum = editLocoNum + 10;
                editLocoNumBox();
            }
            break;
        case 47:
            if(editLocoNum < 9999){
                editLocoNum = editLocoNum + 1;
                editLocoNumBox();
            }
            break;
        case 48:
            if(editLocoNum > 1000){
                editLocoNum = editLocoNum - 1000;
                editLocoNumBox();
            }
            break;
        case 49:
            if(editLocoNum > 100){
                editLocoNum = editLocoNum - 100;
                editLocoNumBox();
            }
            break;
        case 50:
            if(editLocoNum > 10){
                editLocoNum = editLocoNum - 10;
                editLocoNumBox();
            }
            break;
        case 51:
            if(editLocoNum > 1){
                editLocoNum = editLocoNum - 1;
                editLocoNumBox();
            }
            break;
        case 52://exit name editing screen
            buildscreen3();
            break;
        
        case 53://update
            myLayout.setLocoName(locoSelectSelectedLoco, editLocoName);
            buildscreen3();
            break;
        case 55 ... 80:
            if(editCapitals > 0){
                editLocoName = editLocoName + char(box - 55 + 97);
            }else{
                editLocoName = editLocoName + char(box - 55 + 65);
            }
            editLocoNameBox();
            break;
        case 81 ... 96://symbols and numbers
            editLocoName = editLocoName + char(box - 81 + 42);
            editLocoNameBox();
            break;
        case 97:
            editLocoName = editLocoName + char(32);
            editLocoNameBox();
            break;
        case 98:
            editLocoName.trim();
            editLocoName = editLocoName.substring(0,editLocoName.length() - 1);
            editLocoNameBox();
            break;
        case 99:
            if(editCapitals > 0){
                editCapitals = 0;
            }else{
                editCapitals = 1;    
            }
            editLocoLetterBoxes();
            break;
        case 100: //not in use
          //buildscreen6();//consist screen
          break;
        case 101://exit back to screen 1
          buildscreen1();
          break;
        case 102://Clear consist
          for(q=0;q<200;q++){//work through the whole loco array
          //myConstState = myLayout.locoGetConsistState(q);
            if(myLayout.locoGetConsistState(q) == 2){
              myLayout.locoSetConsistState(q,0);//reset all the consist bytes
            }else{
              if(myLayout.locoGetConsistState(q) == 3){
                myLayout.locoSetConsistState(q,1);//reset all the consist bytes
              }
            }
          }
          locoConsistBoxes();
          break;
        case 103 ... 110://consist direction buttons
          //update the direction button
          //get the current direction
          
          myConstState = myLayout.locoGetConsistState(box - 103 + locoConsistScrollOffset);//get the loco consist state
          //Serial.println(myConstState);
          switch(myConstState){
            case 1:
                myLayout.locoSetConsistState(box - 103 + locoConsistScrollOffset,0);
              break;
            case 2:
                myLayout.locoSetConsistState(box - 103 + locoConsistScrollOffset,3);
              break;
            case 3:
                myLayout.locoSetConsistState(box - 103 + locoConsistScrollOffset,2);
              break;
            default://0
                myLayout.locoSetConsistState(box - 103 + locoConsistScrollOffset,1);
              break;
          }
          locoConsistBoxes();
          break;
        case 111 ... 118://consist Loco names
          myConstState = myLayout.locoGetConsistState(box - 111 + locoConsistScrollOffset);//get the loco consist state
          Serial.println(myConstState);
          switch(myConstState){
            case 1:
                myLayout.locoSetConsistState(box - 111 + locoConsistScrollOffset,3);
              break;
            case 2:
                myLayout.locoSetConsistState(box - 111 + locoConsistScrollOffset,0);
              break;
            case 3:
                myLayout.locoSetConsistState(box - 111 + locoConsistScrollOffset,1);
              break;
            default://0
                myLayout.locoSetConsistState(box - 111 + locoConsistScrollOffset,2);
              break;
          }
          locoConsistBoxes();
          break;

        case 119://scroll up
            if(locoConsistScrollOffset > 8){
                locoConsistScrollOffset = locoConsistScrollOffset - 8;
            }else{
                locoConsistScrollOffset = 0;
            }
            locoConsistBoxes();
            break;
        case 120://scroll down
            if(locoConsistScrollOffset < 200 - 16){
                locoConsistScrollOffset = locoConsistScrollOffset + 8;
            }else{
                locoConsistScrollOffset = 200 - 9;
            }
            //Serial.println(locoSelectScrollOffset);
            locoConsistBoxes();
            break;
        case 121://TX loco data
            myLayout.locoTXLocoData(locoSelectSelectedLoco);
            break;
        case 122://RX loco data
            if(locoRXbuttonState > 0){
              locoRXbuttonState = 0;
            }else{
              locoRXbuttonState = 1; 
            }
            //set the system ready to receive
            myLayout.setLocoRXState(locoRXbuttonState, locoSelectSelectedLoco);//set the system to receive or NOT receive data
            //Update the buttons
            screen3TXRXbuttons();
           
            break;
          case 123://<<clock
            clockSpeed = myLayout.rtcClockSpeed();//get current clock speed
            if(clockSpeed > 0){
             clockSpeed = clockSpeed - 1;
             myLayout.sendClockSpeedChange(clockSpeed);//transmit new clock speed
            }
            break;
          case 124://clock >>
            clockSpeed = myLayout.rtcClockSpeed();//get current clock speed
            if(clockSpeed < 255){
              clockSpeed = clockSpeed + 1;
              myLayout.sendClockSpeedChange(clockSpeed);//transmit new clock speed
            }
            break;
            break;
          case 125://Exit
            buildscreen1();
            break;
           case 126 ... 127://Layout Code
            buildscreen8();
            break;
          case 128://txAll
            if(rxAllState < 1){//cannot start transmitting while doing bulk RX
              txAllState = 1;
              buildscreen7();//redraw button in yellow
              myLayout.locoTXAllLocoData(); //transmit all loco data
              txAllState = 0;//reset button
              buildscreen7();
            }
            break;
          case 129://rxAll... only trigger if not doing tx
          
            if(txAllState < 1){
              if(rxAllState > 0){
                rxAllState = 0;
                myLayout.locoRXAllLocoData(0);//stop ability to receive
              }else{
                rxAllState = 1;
                myLayout.locoRXAllLocoData(1);//start ability to receive
              }
              
              buildscreen7();
              
            }
            
            break;
          case 130 ... 133:
            myLayout.setnowRailAddr(box - 130, 1);//increase value
            buildScreen8CodeBox();
            break;
          case 135 ... 138:
            myLayout.setnowRailAddr(box - 135, -1);//decrease value
            buildScreen8CodeBox();
            break;
          case 139:
            buildscreen7();
            break;
          case 141:
            buildscreen9();//acc command screen
            break;
          case 142:
            buildscreen1();//back to main screen
            break;
          case 143: //on
           
            sendStaticAccNum = (staticAccNum[3] * 1000) + (staticAccNum[2] * 100) + (staticAccNum[1] * 10) + staticAccNum[0];
            myLayout.sendAccessoryCommand(sendStaticAccNum, 1, MESSRESPREQ); 
            onOffState[0] = 1;
            onOffState[1] = 0;
            screen9OnOff();
            break;
          case 144:
            sendStaticAccNum = (staticAccNum[3] * 1000) + (staticAccNum[2] * 100) + (staticAccNum[1] * 10) + staticAccNum[0];
            myLayout.sendAccessoryCommand(sendStaticAccNum, 0, MESSRESPREQ);
            onOffState[0] = 0;
            onOffState[1] = 1;
            screen9OnOff();
            break;  //off
          case 147 ... 156: //screen 9 number buttons
            staticAccNum[3] = staticAccNum[2];
            staticAccNum[2] = staticAccNum[1];
            staticAccNum[1] = staticAccNum[0];
            staticAccNum[0] = box - 147;
            screen9AccNumber();//update screen
            if(onOffState[0] > 0 || onOffState[1] > 0 ){
              onOffState[0] = 0;
              onOffState[1] = 0;
              screen9OnOff();
            }
            break;
          case 157:
            buildscreen10();
            break;
          case 159:
            myLayout.changeWifiChannel(1);//send change channel 
            
            buildscreen1();//return to front screen
            break;
          case 160:
            myLayout.changeWifiChannel(6);//send change channel 
            buildscreen1();//return to front screen
            break;
          case 161:
            myLayout.changeWifiChannel(11);//send change channel 
            buildscreen1();//return to front screen
            break;
          case 162:
            buildscreen1();//return to front screen
            break;
        default:
            break;
    }
  }
  
}

void touchProcess() {
  int q;
  int xTouch;
  int yTouch;
  //deal with screen presses
  if (currentMillis - screenMillis >= screenTimer) {
    if (ts.touched()) {
      screenMillis = currentMillis;  //reset debounce only if screen pressed
      // Retrieve a point
      TS_Point p = ts.getPoint();

      xTouch = p.x;
      xTouch = map(xTouch, 316, 10, 0, 319);
      if (xTouch < 0) {
        xTouch = 0;
      }
      if (xTouch > 319) {
        xTouch = 319;
      }
      //Serial.println("x:" + String(xTouch));

      yTouch = p.y;
      yTouch = map(yTouch, 463, 2, 0, 479);
      if (yTouch < 0) {
        yTouch = 0;
      }
      if (yTouch > 479) {
        yTouch = 479;
      }
      //Serial.println("y:" + String(yTouch));
      //now see if box touched
      for (q = 0; q < numScreenBoxes; q++) {

        if (screenBoxes[q][4] == screenMode) {  //check box is correct screen mode

          //now check touch is within box limits
          if (xTouch > screenBoxes[q][0] && xTouch < screenBoxes[q][0] + screenBoxes[q][2] && yTouch > screenBoxes[q][1] && yTouch < screenBoxes[q][1] + screenBoxes[q][3]) {

            if (q != lastBox) {  // if a different box
              touchProcessBox(q);
              lastBox = q;
            } else {
              if (currentMillis - doublePressTimer >= doublePress) {  //extra debounce
                doublePressTimer = currentMillis;
                touchProcessBox(q);
              }
            }
          }
        }
      }
    }
  }
  //deal with resetting recallTimer
  if(recallPos > 1){//only needs to run if recallPos > 1... otherwise doesn't need resetting
    if(currentMillis - recallMillis > recallTimer){
      recallPos = 1;//reset position
      //redraw the buitton
      if(screenMode == 1){
        String myString;
        int box;
        box = 18; //recall buttons
        myString = "Recall";
        drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 35, 20);  
      }
    }
  }
}

void touchSetUp() {
  if (!ts.begin(40)) {
    Serial.println("Unable to start the capacitive touchscreen.");
  }
}