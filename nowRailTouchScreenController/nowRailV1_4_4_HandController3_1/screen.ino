

int locoSelectSelectedLoco;  //gets the selected loco in loco select screen
int locoSelectScrollOffset;
int locoConsistScrollOffset;
int editLocoNum;
String editLocoName;
byte screenMode;  //Keeps track of screen mode
byte editCapitals;
byte isConsist;
byte txAllState;

byte txNum;  //current slot transmitted
byte rxNum;  //current slot received

byte staticAccNum[4];  //stores static acc address for screen 9

//loco recall
byte recallPos = 1;
unsigned long recallMillis;
int recallTimer = 3000;  //will reset the recall pos to 1 after 3 seconds

byte onOffState[2];

byte currentScreenDay = 10;     //set out of bounds so it triggers on start up
int numScreenBoxes = 163;       //number of boxes in screen box array
int screenBoxes[][6] = {        //x,y,w,h,screen mode,state 0 = off 1 = selected 2 = red
  { 101, 0, 115, 24, 0, 0 },    //day 0
  { 0, 0, 100, 24, 0, 0 },      //time 1
  { 260, 0, 80, 24, 0, 0 },     //time speed 2
  { 0, 25, 319, 54, 1, 0 },     //Loco name/number box 3
  { 186, 50, 132, 28, 1, 0 },   //Loco Speed 4
  { 0, 80, 64, 64, 1, 1 },      //Functions 0 Lights  5
  { 64, 80, 64, 64, 1, 0 },     //Functions 1        6
  { 128, 80, 64, 64, 1, 0 },    //Functions 2       7
  { 192, 80, 64, 64, 1, 0 },    //Functions 3       8
  { 256, 80, 64, 64, 1, 1 },    //Functions 4       9
  { 0, 144, 64, 64, 1, 0 },     //Functions 5        10
  { 64, 144, 64, 64, 1, 0 },    //Functions 6        11
  { 128, 144, 64, 64, 1, 1 },   //Functions 7       12
  { 192, 144, 64, 64, 1, 0 },   //Functions 8       13
  { 256, 144, 64, 64, 1, 0 },   //Functions 9       14
  { 0, 208, 159, 64, 1, 0 },    //Functions <<        15
  { 160, 208, 160, 64, 1, 0 },  //Functions >>        16
  { 160, 272, 160, 64, 1, 0 },  //Change Loco        17
  //{ 0, 336, 159, 64, 1, 0 },    //Clock <<        18
  //{ 160, 336, 160, 64, 1, 0 },  //Clock >>        19
  { 0, 336, 159, 64, 1, 0 },    //Recall<        18
  { 160, 336, 160, 64, 1, 0 },  //Consist        19
  //{ 0, 400, 159, 64, 1, 0 },    //Recall       20
  { 0, 400, 159, 64, 1, 0 },    //Settings       20
  { 160, 400, 160, 64, 1, 0 },  //STOP        21
  // all screens
  { 0, 80, 320, 400, 0, 0 },  //22 black background behind screen
  //screen 2 loco select
  { 0, 415, 159, 64, 2, 0 },    //Exit      23
  { 160, 415, 160, 64, 2, 0 },  //Select        24


  { 0, 82, 279, 41, 2, 0 },  //locos to select 25
  { 0, 123, 279, 41, 2, 0 },
  { 0, 164, 279, 41, 2, 0 },
  { 0, 205, 279, 41, 2, 0 },
  { 0, 246, 279, 41, 2, 0 },
  { 0, 287, 279, 41, 2, 0 },
  { 0, 328, 279, 41, 2, 0 },
  { 0, 369, 279, 41, 2, 0 },  //locos to select 32


  { 280, 82, 40, 82, 2, 0 },   //scroll Up arrow 33
  { 280, 328, 40, 82, 2, 0 },  //scroll down arrow 34
  //screen 3
  { 0, 415, 159, 64, 3, 0 },    //Back      35
  { 160, 415, 160, 64, 3, 0 },  //Select        36
  { 0, 351, 159, 64, 3, 0 },    //Send    37
  { 160, 351, 160, 64, 3, 0 },  //Edit       38
  { 0, 144, 320, 64, 3, 0 },    //Loco Num    39
  { 0, 208, 320, 64, 3, 0 },    //Loco Name      40
  //screen 4
  { 0, 415, 159, 64, 4, 0 },    //Back      41
  { 160, 415, 160, 64, 4, 0 },  //Update        42
  { 32, 208, 256, 64, 4, 0 },   //loco num  43
  { 32, 144, 64, 64, 4, 0 },    //loco 1st digit inc 44
  { 96, 144, 64, 64, 4, 0 },    //loco 2nd digit inc       45
  { 160, 144, 64, 64, 4, 0 },   //loco 3rd  inc     46
  { 224, 144, 64, 64, 4, 0 },   //loco 4th  inc     47
  { 32, 272, 64, 64, 4, 1 },    //loco 1st digit dec 48
  { 96, 272, 64, 64, 4, 0 },    //loco 2nd digit dec       49
  { 160, 272, 64, 64, 4, 0 },   //loco 3rd  dec     50
  { 224, 272, 64, 64, 4, 0 },   //loco 4th  dec     51
  //screen 5
  { 0, 415, 159, 64, 5, 0 },    //Back      52
  { 160, 415, 160, 64, 5, 0 },  //Update        53
  { 0, 80, 320, 53, 5, 0 },     //loco name 54
  { 0, 133, 40, 47, 5, 0 },     //row 1 letters A 55
  { 40, 133, 40, 47, 5, 0 },
  { 80, 133, 40, 47, 5, 0 },
  { 120, 133, 40, 47, 5, 0 },
  { 160, 133, 40, 47, 5, 0 },
  { 200, 133, 40, 47, 5, 0 },
  { 240, 133, 40, 47, 5, 0 },
  { 280, 133, 40, 47, 5, 0 },  //end row 1 H 62
  { 0, 180, 40, 47, 5, 0 },    //row 2 letters I 63
  { 40, 180, 40, 47, 5, 0 },
  { 80, 180, 40, 47, 5, 0 },
  { 120, 180, 40, 47, 5, 0 },
  { 160, 180, 40, 47, 5, 0 },
  { 200, 180, 40, 47, 5, 0 },
  { 240, 180, 40, 47, 5, 0 },
  { 280, 180, 40, 47, 5, 0 },  //end row 2 P 70
  { 0, 227, 40, 47, 5, 0 },    //row 3 letters Q 71
  { 40, 227, 40, 47, 5, 0 },
  { 80, 227, 40, 47, 5, 0 },
  { 120, 227, 40, 47, 5, 0 },
  { 160, 227, 40, 47, 5, 0 },
  { 200, 227, 40, 47, 5, 0 },
  { 240, 227, 40, 47, 5, 0 },
  { 280, 227, 40, 47, 5, 0 },  //end row 3 X 78
  { 0, 274, 40, 47, 5, 0 },    //row 4 letters Y 79
  { 40, 274, 40, 47, 5, 0 },   //Z 80
  { 80, 274, 40, 47, 5, 0 },   // * Asc 42
  { 120, 274, 40, 47, 5, 0 },  // +
  { 160, 274, 40, 47, 5, 0 },  // ,
  { 200, 274, 40, 47, 5, 0 },  // -
  { 240, 274, 40, 47, 5, 0 },  // .
  { 280, 274, 40, 47, 5, 0 },  //end row 4 / Asc 47 86
  { 0, 321, 40, 47, 5, 0 },    //row 5 letters 0 87
  { 40, 321, 40, 47, 5, 0 },   //1
  { 80, 321, 40, 47, 5, 0 },   // 2
  { 120, 321, 40, 47, 5, 0 },  // 3
  { 160, 321, 40, 47, 5, 0 },  // 4
  { 200, 321, 40, 47, 5, 0 },  // 5
  { 240, 321, 40, 47, 5, 0 },  // 6
  { 280, 321, 40, 47, 5, 0 },  //end row 5 7 94
  { 0, 368, 40, 47, 5, 0 },    //row 6letters 8 95
  { 40, 368, 40, 47, 5, 0 },   //9 96
  { 80, 368, 90, 47, 5, 0 },   // space 97
  { 170, 368, 90, 47, 5, 0 },  // Del 98
  { 260, 368, 60, 47, 5, 0 },  // Case 99
  //screen 3 mods for consist
  { 0, 272, 320, 64, 3, 0 },  //Set Consist      100........no longer in use

  //screen 6 consist selection
  { 0, 415, 159, 64, 6, 0 },    //Exit      101
  { 160, 415, 160, 64, 6, 0 },  //Clr Consist        102

  { 0, 82, 50, 41, 6, 0 },  //103 locos to select Direction...small button on left
  { 0, 123, 50, 41, 6, 0 },
  { 0, 164, 50, 41, 6, 0 },
  { 0, 205, 50, 41, 6, 0 },
  { 0, 246, 50, 41, 6, 0 },
  { 0, 287, 50, 41, 6, 0 },
  { 0, 328, 50, 41, 6, 0 },
  { 0, 369, 50, 41, 6, 0 },  //110  locos to select Direction...small button on left

  { 52, 82, 227, 41, 6, 0 },  //111 locos to select 111
  { 52, 123, 227, 41, 6, 0 },
  { 52, 164, 227, 41, 6, 0 },
  { 52, 205, 227, 41, 6, 0 },
  { 52, 246, 227, 41, 6, 0 },
  { 52, 287, 227, 41, 6, 0 },
  { 52, 328, 227, 41, 6, 0 },
  { 52, 369, 227, 41, 6, 0 },  //118 locos to select 118


  { 280, 82, 40, 82, 6, 0 },   //scroll Up arrow 119
  { 280, 328, 40, 82, 6, 0 },  //scroll down arrow 120
  //
  { 0, 80, 128, 64, 3, 0 },    //TX loco details      121
  { 192, 80, 128, 64, 3, 0 },  //RX loco details      122

  //screen 7 settings
  { 0, 212, 159, 64, 7, 0 },    //Clock <<        123
  { 160, 212, 160, 64, 7, 0 },  //Clock >>        124
  { 0, 415, 159, 64, 7, 0 },    //Exit      125
  { 0, 80, 320, 64, 7, 0 },     //Layout Code 126
  { 0, 146, 320, 64, 7, 0 },    //Layout Code Num 127
  { 0, 348, 159, 64, 7, 0 },    //TX All        128
  { 160, 348, 160, 64, 7, 0 },  //RX All        129
  //Screen 7 ...WIFI channel change...button 157
  //screen 8 change layout Code
  { 32, 144, 64, 64, 8, 0 },   //layout addr 1st digit inc 130
  { 96, 144, 64, 64, 8, 0 },   //layout addr 2nd digit inc       131
  { 160, 144, 64, 64, 8, 0 },  //layout addr 3rd  inc     132
  { 224, 144, 64, 64, 8, 0 },  //layout addr 4th  inc     133

  { 0, 210, 320, 64, 7, 0 },  //layout code 134

  { 32, 276, 64, 64, 8, 0 },    //layout addr 1st digit dec 135
  { 96, 276, 64, 64, 8, 0 },    //layout addr 2nd digit dec 136
  { 160, 276, 64, 64, 8, 0 },   //layout addr 3rd  dec 137
  { 224, 276, 64, 64, 8, 0 },   //layout addr 4th  dec  138
  { 0, 415, 159, 64, 8, 0 },    //Exit      139
  { 160, 415, 160, 64, 8, 0 },  //UPdate 140... not used
  //addition to screen 1
  { 0, 272, 159, 64, 1, 0 },  //Acc Command        141
  //Screen 9
  { 0, 400, 159, 64, 9, 0 },    //Exit      142
  { 0, 336, 159, 64, 9, 0 },    //On        143
  { 160, 336, 160, 64, 9, 0 },  //Off        144
  { 0, 80, 320, 64, 9, 0 },     //Title Acc Number 145
  { 0, 144, 320, 64, 9, 0 },    //current number entered 146

  { 0, 208, 64, 64, 9, 0 },    // 0   147
  { 64, 208, 64, 64, 9, 0 },   // 1
  { 128, 208, 64, 64, 9, 0 },  // 2
  { 192, 208, 64, 64, 9, 0 },  // 3
  { 256, 208, 64, 64, 9, 0 },  // 4
  { 0, 272, 64, 64, 9, 0 },    // 5
  { 64, 272, 64, 64, 9, 0 },   // 6
  { 128, 272, 64, 64, 9, 0 },  // 7
  { 192, 272, 64, 64, 9, 0 },  // 8
  { 256, 272, 64, 64, 9, 0 },  // 9       156
  //screen 7 addition
  { 0, 278, 160, 64, 7, 0 },  //WIFI CH       157
  //screen 10 wifi channel
  { 0, 80, 320, 64, 10, 0 },     //Select channel Layout Code 158
  { 0, 148, 64, 64, 10, 0 },      //channel 1   159
  { 128, 148, 64, 64, 10, 0 },    //channel 6   160     
  { 256, 148, 64, 64, 10, 0 },    //channel 11  161 
  { 0, 400, 159, 64, 10, 0 },    //Exit      162    
  { 0, 0, 0, 0, 0, 0 }
};

void buildscreen10() {  //wifi seclection
  int box;
  int q;
  String myString;
  screenMode = 10;  //Screen Mode
  tft.setFreeFont(FSS12);
  screenBlackBack();

  box = 158;
  myString = "Select WiFi Channel";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 159;
  myString = "1";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  box = 160;
  myString = "6";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  box = 161;
  myString = "11";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 162;
  myString = "Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  

  
}


void screen9AccNumber() {
  int box;
  int q;
  String myString;

  box = 146;
  q = (staticAccNum[3] * 1000) + (staticAccNum[2] * 100) + (staticAccNum[1] * 10) + staticAccNum[0];
  myString = stringPad('0', 4, String(q));
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

//sets the ON/OFF buttons
void screen9OnOff() {
  int box;
  String myString;
  //onOffState[1]
  box = 143;
  myString = "ON";


  if (onOffState[0] < 1) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 0, CC_DATUM);
  }


  box = 144;
  myString = "OFF";

  if (onOffState[1] < 1) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 0, CC_DATUM);
  }
}

void buildscreen9() {  //acc command screen
  int box;
  int q;
  String myString;
  screenMode = 9;  //Screen Mode
  tft.setFreeFont(FSS12);
  screenBlackBack();

  box = 142;
  myString = "Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  screen9OnOff();

  box = 145;
  myString = "Acc Number";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  screen9AccNumber();  //sorts the displayed nummber

  for (q = 0; q < 10; q++) {
    box = q + 147;
    myString = String(q);
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  }
}

//Layout Code
void buildScreen8CodeBox() {

  int box = 134;
  String myString = myLayout.getnowRailAddr();
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

void buildscreen8() {
  int box;
  String myString;
  screenMode = 8;  //Layout Code
  tft.setFreeFont(FSS12);
  screenBlackBack();

  myString = '+';
  for (box = 130; box < 134; box++) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  }

  buildScreen8CodeBox();

  myString = '-';
  for (box = 135; box < 139; box++) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  }

  box = 139;
  myString = "Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

//Settings
void buildscreen7() {
  int box;
  String myString;
  screenMode = 7;  //settings
  tft.setFreeFont(FSS12);
  screenBlackBack();

  box = 123;  //exit
  myString = "<< Clock";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 124;  //select
  myString = "Clock >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 125;  //select
  myString = "Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 126;  //select
  myString = "Layout Code";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 127;  //select
  myString = myLayout.getnowRailAddr();
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 128;  //exit
  myString = "TX ALL";
  if (txAllState < 1) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 0, CC_DATUM);
  }

  box = 129;  //select
  myString = "RX ALL";
  if (rxAllState < 1) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 0, CC_DATUM);
  }

  box = 157;  //select
  myString = "Wifi CH";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

void setNewCurrentLoco() {

  myLayout.locoReCallDataUpdate(locoID, locoSpeed);  //Update the recall system...current loco
  //stop current loco

  
  //now set the new loco
  locoSpeed = 128;  //set speed to 0 forwards
  locoID = locoSelectSelectedLoco;
  myLayout.locoReCallDataUpdate(locoID, locoSpeed);  //Update the recall system...new loco
  screenLocoNameNum();
  screenLocoSpeed();
  buildscreen1();
}


//deals with loco recall button press
void locoRecallPressed() {
  //update the current locos speeds
  myLayout.locoReCallDataUpdate(locoID, locoSpeed);  //Update the recall system...current loco
  //now set for new loco
  locoID = myLayout.locoReCallGetLocoID(recallPos);
  locoSpeed = myLayout.locoReCallGetLocoSpeed(recallPos);
  recallPos = recallPos + 2;
  recallMillis = currentMillis;  //sets for 5 sec timer

  //Now I have this data update the screen and recall system
  myLayout.locoReCallDataUpdate(locoID, locoSpeed);  //Update the recall system...current loco
  screenLocoNameNum();
  screenLocoSpeed();
  buildscreen1();
}

void locoConsistBoxes() {
  byte q;
  int box;
  String myString;
  byte myConstState;

  for (q = 0; q < 8; q++) {
    box = q + 103;                                                             //direction box
    myConstState = myLayout.locoGetConsistState(q + locoConsistScrollOffset);  //get the loco consist state
    //myString = String(myConstState);
    if (myConstState == 0 || myConstState == 2) {
      myString = 'F';
      drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
    } else {
      myString = 'R';
      drawScreenBoxSmallfnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 0, CC_DATUM);
    }

    box = q + 111;  //direction box
    //myString = "1234 ABCDEFGHIJ";///reduced line length
    myString = stringPad('0', 4, String(myLayout.getLocoDCCAddress(q + locoConsistScrollOffset))) + ' ' + myLayout.getLocoName(q + locoConsistScrollOffset);
    myString = myString.substring(0, 15);
    if (myConstState < 2) {  //not in consist
      drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 2, CL_DATUM);
    } else {
      drawScreenBoxSmallfnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 2, CL_DATUM);
    }
  }

  //Loco scroll buttons
  box = 119;  //scroll up
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
  tft.fillTriangle(screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1], screenBoxes[box][0], screenBoxes[box][1] + screenBoxes[box][3], screenBoxes[box][0] + screenBoxes[box][2], screenBoxes[box][1] + screenBoxes[box][3], TFT_WHITE);

  box = 120;  //scroll down
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
  tft.fillTriangle(screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1] + screenBoxes[box][3], screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][0] + screenBoxes[box][2], screenBoxes[box][1], TFT_WHITE);
}

//consist screen
void buildscreen6() {
  int box;
  String myString;
  screenMode = 6;  //Loco consist screen
  tft.setFreeFont(FSS12);
  screenBlackBack();

  box = 101;  //exit
  myString = "<< Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 102;  //select
  myString = "Clr Consist";
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  locoConsistBoxes();
}


void editLocoLetterBoxes() {
  int box;
  int q;
  String myString;
  for (q = 0; q < 26; q++) {
    box = q + 55;
    if (editCapitals < 1) {
      myString = char(q + 65);
    } else {
      myString = char(q + 97);
    }
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 6, 12);
  }

  box = 99;
  if (editCapitals < 1) {
    myString = "abc";
  } else {
    myString = "ABC";
  }
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

void editLocoNameBox() {
  int box;
  String myString;
  box = 54;
  if (editLocoName.length() > 17) {
    editLocoName = editLocoName.substring(0, 17);
  }
  myString = editLocoName;
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);
}

//Loco Name editing
void buildscreen5() {  //edit loco name
  int box;
  int q;
  String myString;
  screenMode = 5;  //Confirm Loco Select
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  screenBlackBack();
  box = 52;
  myString = "<< Back";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 10, 20);

  box = 53;
  myString = "Update >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 9, 20);

  editLocoName = myLayout.getLocoName(locoSelectSelectedLoco);
  editLocoName.trim();
  editLocoNameBox();
  editLocoLetterBoxes();

  for (q = 0; q < 16; q++) {
    box = q + 81;
    myString = char(q + 42);
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 12, 12);
  }

  box = 97;
  myString = "Space";
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);

  box = 98;
  myString = "< Del";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 3, 12);
}


//for screen 4 loco number editing
void editLocoNumBox() {
  int box;
  String myString;
  box = 43;
  myString = stringPad('0', 4, String(editLocoNum));
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 102, 20);
}
//loco address editing
void buildscreen4() {
  int box;
  int q;
  String myString;
  screenMode = 4;  //Confirm Loco Select
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);

  screenBlackBack();
  box = 41;
  myString = "<< Back";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 10, 20);

  box = 42;
  myString = "Update >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 9, 20);


  editLocoNum = myLayout.getLocoDCCAddress(locoSelectSelectedLoco);
  editLocoNumBox();
  for (q = 0; q < 4; q++) {
    box = q + 44;
    myString = "+";
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 25, 20);
  }
  for (q = 0; q < 4; q++) {
    box = q + 48;
    myString = "-";
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 26, 20);
  }
}

//View selected loco details
void buildscreen3() {
  int box;
  int q;

  String myString;

  screenMode = 3;  //Confirm Loco Select
  screenBlackBack();
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  box = 35;  //exit
  myString = "<< Back";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 10, 20);

  box = 36;  //select
  myString = "Select >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);

  //box = 37;  //recieve...incoming data
  //myString = "Recieve <<";
  //myString = " ";
  //drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);

  //box = 38;  //send...transmit out
  //myString = "  Send >>";
  //myString = " ";
  //drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);

  box = 39;  //loco number
  myString = stringPad('0', 4, String(myLayout.getLocoDCCAddress(locoSelectSelectedLoco)));
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);

  box = 40;  //loconame
  myString = myLayout.getLocoName(locoSelectSelectedLoco);
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);



  screen3TXRXbuttons();
}
//buttons 121 and 122 TX and RX
void screen3TXRXbuttons() {
  int box;
  String myString;
  if (screenMode == 3) {  //only action if screen mode is still 3
    box = 121;            //TX
    myString = "TX >";
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 20, 20);

    box = 122;  //TX
    myString = " > RX";
    if (locoRXbuttonState < 1) {  //not waiting for transmission
      drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 20, 20);
    } else {  //waiting for transmission
      drawScreenBox30fnt(box, TFT_RED, TFT_BLACK, myString, TFT_BLACK, TFT_RED, 20, 20);
    }
  }
}

//buttons with loco nmes to select
void locoSelectBoxes() {
  byte q;
  int box;
  String myString;

  // locos
  for (q = 0; q < 8; q++) {
    box = q + 25;

    myString = stringPad('0', 4, String(myLayout.getLocoDCCAddress(q + locoSelectScrollOffset))) + ' ' + myLayout.getLocoName(q + locoSelectScrollOffset);
    //myString = "1234 ABCDEFGHIJKLMNOPQR";
    myString = myString.substring(0, 24);
    if (q + locoSelectScrollOffset != locoSelectSelectedLoco) {
      // drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 1, 9);
      drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 2, CL_DATUM);
    } else {
      // drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 1, 9);
      drawScreenBoxSmallfnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 2, CL_DATUM);
    }
  }
  //Loco scroll buttons
  box = 33;  //scroll up
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
  tft.fillTriangle(screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1], screenBoxes[box][0], screenBoxes[box][1] + screenBoxes[box][3], screenBoxes[box][0] + screenBoxes[box][2], screenBoxes[box][1] + screenBoxes[box][3], TFT_WHITE);

  box = 34;  //scroll down
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
  tft.fillTriangle(screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1] + screenBoxes[box][3], screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][0] + screenBoxes[box][2], screenBoxes[box][1], TFT_WHITE);
}

void buildscreen2() {
  int box;
  int q;
  int w;
  String myString;

  screenMode = 2;  //Loco select screen
  screenBlackBack();

  box = 23;  //exit
  myString = "<< Exit";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 27, 20);

  box = 24;  //select
  myString = "Select >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 17, 20);

  locoSelectSelectedLoco = 1000;  // reset ready for selection
  locoSelectBoxes();
}

// other screen 1 buttons
void screen1OtherButtons() {
  int q;
  String myString;
  int box = 15;
  myString = "<< Func";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 16, 20);

  box = 16;
  myString = "Func >>";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 22, 20);

  box = 17;
  myString = "Change Loco";
  //drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 63, 20);
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);


  box = 18;
  myString = "Recall";
  if (recallPos < 2) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 35, 20);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_BLACK, myString, TFT_BLACK, TFT_YELLOW, 35, 20);
  }

  box = 19;
  isConsist = 0;
  for (q = 0; q < 200; q++) {                   //work through the whole loco array
    if (myLayout.locoGetConsistState(q) > 1) {  //see if any locos in consist mode
      isConsist = 1;
      q = 200;
    }
  }
  myString = "Consist";
  if (isConsist < 1) {
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 65, 20);
  } else {
    drawScreenBox30fnt(box, TFT_YELLOW, TFT_YELLOW, myString, TFT_BLACK, TFT_YELLOW, 65, 20);
  }

  box = 20;  //recall buttons
  myString = "Settings";
  drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 14, 20);

  box = 21;
  Serial.print("PWRSTATE: ");
  Serial.println(powerState);
  if (powerState < 1) {
    myString = "Start";
    drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 40, 20);
  } else {
    myString = "STOP";
    drawScreenBox30fnt(box, TFT_RED, TFT_WHITE, myString, TFT_WHITE, TFT_RED, 43, 20);
  }

  box = 141;
  myString = "Acc Command";
  //drawScreenBox30fnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 63, 20);
  drawScreenBoxSmallfnt(box, TFT_BLACK, TFT_WHITE, myString, TFT_WHITE, TFT_BLACK, 0, CC_DATUM);
}

//Draws an individual function box
void drawFunctionBox(int func) {
  String myString;
  int box = func + 5 - funcSet;
  //funcSet variable that holds the current function set, can have values 0,10,20
  //Serial.print(funcSet);



  switch (func) {
    case 0:
      myString = 'L';
      break;
    case 1:
      //myString = "WH"
      myString = '1';
      ;
      break;
    case 2:
      //myString = "SQ";
      myString = '2';
      break;
    case 3:
      // myString = "CO";
      myString = '3';
      break;
    case 4:
      myString = '4';
      break;
    case 5:
      myString = '5';
      break;
    case 6:
      myString = '6';
      break;
    case 7:
      myString = '7';
      break;
    case 8:
      myString = '8';
      break;
    case 9:
      myString = '9';
      break;
    default:
      myString = String(func);
      break;
  }



  if (myLayout.getLocoDCCFuncState(locoID, func) < 1) {  //check the values in the loco array for current loco
    tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
    tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  } else {  //selected
    tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_YELLOW);
    tft.setTextColor(TFT_BLACK, TFT_YELLOW);
  }

  //sets the cursor position, needs to change for numbers greater than 1 char in length
  // if (funcSet > 0) {
  //   //tft.setCursor(screenBoxes[box][0] + 12, screenBoxes[box][1] + 20);
  //   tft.setCursor(screenBoxes[box][0] + 12, screenBoxes[box][1] + 42);
  // } else {
  //   //tft.setCursor(screenBoxes[box][0] + 22, screenBoxes[box][1] + 20);
  //   tft.setCursor(screenBoxes[box][0] + 22, screenBoxes[box][1] + 42);
  // }
  tft.setFreeFont(FSS18);
  tft.setTextDatum(CC_DATUM);  //centre text
  //tft.println(myString);
  tft.drawString(myString, screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1] + (screenBoxes[box][3] / 2));

  //tft.println(myString);
}

//Draws all the function boxes
void screen1LocoFunctions() {
  int q;
  for (q = 0; q < 10; q++) {
    drawFunctionBox(q + funcSet);  //draw all the function boxes
  }
}
//sets black background
void screenBlackBack() {
  int box = 22;
  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
}

//Main loco control screen
void buildscreen1() {
  screenMode = 1;
  screenBlackBack();  //sets the black background before new screen is drawn
  screen1LocoFunctions();
  screen1OtherButtons();
}

//30 font function button
void drawScreenBox30fnt(int box, int boxColour, int boxBorder, String myString, int textColour, int textBackgrnd, int textXOffset, int textYOffset) {
  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], boxColour);
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], boxBorder);
  //tft.setCursor(screenBoxes[box][0] + textXOffset, screenBoxes[box][1] + textYOffset);
  //tft.setCursor(screenBoxes[box][0] + textXOffset, screenBoxes[box][1] + textYOffset + 21);

  tft.setTextColor(textColour, textBackgrnd);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS18);
  tft.setTextDatum(CC_DATUM);
  //tft.println(myString);
  tft.drawString(myString, screenBoxes[box][0] + (screenBoxes[box][2] / 2), screenBoxes[box][1] + (screenBoxes[box][3] / 2));
}

//12 font function button
void drawScreenBoxSmallfnt(int box, int boxColour, int boxBorder, String myString, int textColour, int textBackgrnd, int textXOffset, int alignment) {
  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], boxColour);
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], boxBorder);
  //tft.setCursor(screenBoxes[box][0] + textXOffset, screenBoxes[box][1] + textYOffset);
  //tft.setCursor(screenBoxes[box][0] + textXOffset, screenBoxes[box][1] + textYOffset + 21);
  tft.setTextColor(textColour, textBackgrnd);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  switch (alignment) {
    case CC_DATUM:  //centred text
      tft.setTextDatum(CC_DATUM);
      tft.drawString(myString, screenBoxes[box][0] + (screenBoxes[box][2] / 2) + textXOffset, screenBoxes[box][1] + (screenBoxes[box][3] / 2));
      break;
    case CL_DATUM:
      tft.setTextDatum(CL_DATUM);
      tft.drawString(myString, screenBoxes[box][0] + textXOffset, screenBoxes[box][1] + (screenBoxes[box][3] / 2));
      break;
    default:

      break;  //do nothing
  }

  //tft.println(myString);
}

void screenTimeAcc() {
  int box = 2;
  String myString;
  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
  //tft.setTextDatum(TL_DATUM);
  tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 20);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  myString = 'x' + String(myLayout.rtcClockSpeed());
  myString = stringPad(' ', 4, myString);
  tft.println(myString);
}

//updates the screens loco speed
void screenLocoSpeed() {
  int box = 4;
  int displaySpeed;
  int q;
  byte sendDir;
  byte sendConsistRevDirection;
  byte myConstState;
  byte sendSpeed;
  String myString;
  if (locoSpeed < 128) {  //Reverse
    myString = "  REV ";
    sendDir = 0;
    sendSpeed = map(locoSpeed, 0, 127, 127, 0);
    myString = myString + stringPad('0', 3, String(sendSpeed));
    // drawScreenBox30fnt(box, TFT_RED, TFT_RED, myString, TFT_WHITE, TFT_RED, 1, 2);
    //void drawScreenBox30fnt(int box, int boxColour, int boxBorder, String myString, int textColour, int textBackgrnd, int textXOffset, int textYOffset) {

    tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_RED);
    tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_RED);
    tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 22);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setFreeFont(FSS12);
    tft.println(myString);



    //


  } else {  //forward
    myString = "  FWD ";
    sendDir = 1;
    sendSpeed = map(locoSpeed, 128, 255, 0, 127);
    myString = myString + stringPad('0', 3, String(sendSpeed));
    // drawScreenBox30fnt(box, TFT_BLACK, TFT_BLACK, myString, TFT_WHITE, TFT_BLACK, 1, 2);

    tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
    tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
    tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 22);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setFreeFont(FSS12);
    tft.println(myString);
  }
  myLayout.sendDCCLocoSpeed(myLayout.getLocoDCCAddress(locoID), sendSpeed, sendDir);  //send loco speed command out over nowRail
  //now add in the consist locos
  if (sendDir > 0) {
    sendConsistRevDirection = 0;
  } else {
    sendConsistRevDirection = 1;
  }
  for (q = 0; q < 200; q++) {  //work through the whole lcoo array
    myConstState = myLayout.locoGetConsistState(q);
    switch (myConstState) {
      case 2:                                                                          //consist fwd..same direction as main loco
        myLayout.sendDCCLocoSpeed(myLayout.getLocoDCCAddress(q), sendSpeed, sendDir);  //send loco speed command out over nowRail
        break;
      case 3:                                                                                          //consist rev...opposite to main loco
        myLayout.sendDCCLocoSpeed(myLayout.getLocoDCCAddress(q), sendSpeed, sendConsistRevDirection);  //send loco speed command out over nowRail
        break;
      default:  //if not in consist do nothing
        break;
    }
  }
  //
}

//display loco name
void screenLocoNameNum() {
  int box = 3;
  String myString;

  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
  tft.drawRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_WHITE);
  tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);

  myString = myLayout.getLocoName(locoID);  //get the locos name by passing it's number (in this case the selected loco)
  //myString = "ABCDEFGHIJKLMNOPQRS"; //length test

  tft.println(myString);

  //now the loco number
  tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 46);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  //myString = getLocoNumber(systemArray[0]);
  //myString = "0123";
  myString = myLayout.getLocoDCCAddress(locoID);
  myString = stringPad('0', 4, myString);
  tft.println(myString);
}

//time date...top of screen
void buildscreen0() {
  screenLocoNameNum();
  screenLocoSpeed();
  screenTimeAcc();
}

//Initial screen set up
void screenSetUp() {
  locoID = myLayout.locoReCallGetLocoID(0);
  tft.begin();
  tft.init();
  tft.setRotation(2);

  tft.fillScreen(TFT_BLACK);
  buildscreen0();
  buildscreen1();
}

//updates the screen time
void screenTime(byte theHour, byte theMinute, byte theSecond) {
  int box = 1;
  String myString;
  tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
  tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.loadFont(Calibri30);
  tft.setFreeFont(FSS12);
  //myString = stringPad('0', 2, String(theHour));
  myString = stringPad('0', 2, String(theHour)) + ':' + stringPad('0', 2, String(theMinute)) + ':' + stringPad('0', 2, String(theSecond));
  //Serial.println(myString);
  tft.println(myString);
}

String dateGetLayoutDayName(byte theDay) {
  String TheDay;
  switch (theDay) {
    case 0:
      TheDay = " Sunday";
      break;
    case 1:
      TheDay = " Monday";
      break;
    case 2:
      TheDay = " Tuesday";
      break;
    case 3:
      TheDay = " Wednesday";
      break;
    case 4:
      TheDay = " Thursday";
      break;
    case 5:
      TheDay = " Friday";
      break;
    case 6:
      TheDay = " Saturday";
      break;
    default:
      TheDay = "DAY ERROR in dateGetDayName()";
      break;
  }
  return TheDay;
}

//updates the screen day
void screenDay(byte theDay) {
  if (currentScreenDay != theDay) {
    currentScreenDay = theDay;
    String myString = dateGetLayoutDayName(theDay);
    int box = 0;
    tft.fillRect(screenBoxes[box][0], screenBoxes[box][1], screenBoxes[box][2], screenBoxes[box][3], TFT_BLACK);
    tft.setCursor(screenBoxes[box][0] + 1, screenBoxes[box][1] + 20);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    //tft.loadFont(Calibri30);
    tft.setFreeFont(FSS12);
    tft.println(myString);
  }
}

//string padding function
String stringPad(char padChar, byte numPads, String myString) {
  byte stringLength;
  int q;

  stringLength = myString.length();
  for (q = stringLength; q < numPads; q++) {
    myString = padChar + myString;
  }
  return myString;
}