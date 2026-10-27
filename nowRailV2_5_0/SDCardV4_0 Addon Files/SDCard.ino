/*
SDCard.ino v4.0
20/10/26
v4.0
Loco Data... working
v3.0
Sensor to acc commands....working
timedAccessoryStartUp settings...working
MAXREPEATAUTOMATIONS 100 implemented in animation sequences.....working
v2_0 
timed animation sequences...working
V1_0 
The SD card add on allows accessories, Operators timetables and platform displays to be triggered from CSV files on an SD card.

This allows for easy editing without needing to reflash the nowRail ESP32.
*/
//SD Card Reader connections
#define SD_CS 3
#define SPI_MOSI 2
#define SPI_MISO 0
#define SPI_SCK 1

#include "SD.h"

#define MAXREPEATAUTOMATIONS 100  //maximum number of items in repeatAutomation.csv
#define MAXSENSORTOACCS 100       //maximum number of items in sensorToAccCommands.csv

#define NUMOPERATORTIMETABLERESULTS 4  //the maximum number of rows returned from sdTimeTable function
#define NUMPLATFORMDISPLAYRESULTS 6    //the maximum number of rows returned from sdTimeTable function

//This variable can be used to restrict the next timetable call to a specific time...when next train is due
uint16_t nextTrainTime;  //returns next train time from any function call.

//operators Timetable details are returned in these variables
uint16_t opTimeTableReturnedRows;
char opTimeTableTime[NUMOPERATORTIMETABLERESULTS][6];  //5 values stored in HH:MM format
char opTimeTableArrDep[NUMOPERATORTIMETABLERESULTS][11];
char opTimeTableTrainType[NUMOPERATORTIMETABLERESULTS][101];
uint16_t opTimeTablePlatformNum[NUMOPERATORTIMETABLERESULTS];   //stores the platform number
char opTimeTableDestination[NUMOPERATORTIMETABLERESULTS][101];  //train destination
char opTimeTableNotes[NUMOPERATORTIMETABLERESULTS][101];
uint16_t checkTime[2];  //Stores current and last checkTimes

//platform displays results are returned in these variables
uint16_t platformDisplayReturnedRows;
char platformDisplayTime[NUMPLATFORMDISPLAYRESULTS][6];          //5 values stored in HH:MM format
char platformDisplayArrDep[NUMPLATFORMDISPLAYRESULTS][11];       //arrive Dep
uint16_t platformDisplayPlatformNum[NUMPLATFORMDISPLAYRESULTS];  //stores the platform number
char platformDisplayDestination[NUMPLATFORMDISPLAYRESULTS][101];



//my custom variables
uint16_t operatorsNextTrain;  //stores the next time an operators train is due to save transmissions
uint16_t platformDisplayNextTrain;
uint16_t platform1DisplayNextTrain;
uint16_t platform2DisplayNextTrain;



//custom function example to proces results...platforms displays
//processes results for sdPlatformDisplayInfo("TimeTable.csv", 0);  that displays all platform data
void allPlatformDisplayInfoResults() {
  uint16_t q;
  String myString;
  Serial.print("All Platforms ");
  Serial.print("platformDisplayReturnedRows: ");
  Serial.print("nextTrainTime: ");
  Serial.println(nextTrainTime);
  Serial.println(platformDisplayReturnedRows);         //number of results
  platformDisplayNextTrain = nextTrainTime;            //set here to prevent excessive triggering
  for (q = 0; q < platformDisplayReturnedRows; q++) {  //now work through the returned results
    myString = String(platformDisplayTime[q]);         //time string
    Serial.print(myString);
    Serial.print(" : ");
    myString = String(platformDisplayArrDep[q]);  //arr dep
    Serial.print(myString);
    Serial.print(" : ");
    Serial.print(platformDisplayPlatformNum[q]);  //uint16_t platform number 0 if none
    Serial.print(" : ");
    myString = String(platformDisplayDestination[q]);  //destination
    Serial.print(myString);
    Serial.println("  ");
  }
  Serial.println("  ");
}

//results for platform 1
void plat1PlatformDisplayInfoResults() {
  uint16_t q;
  String myString;
  Serial.print("Platform 1 ");
  Serial.print("platformDisplayReturnedRows: ");
  Serial.println(platformDisplayReturnedRows);  //number of results
  Serial.print("nextTrainTime: ");
  Serial.println(nextTrainTime);
  platform1DisplayNextTrain = nextTrainTime;           //set here to prevent excessive triggering
  for (q = 0; q < platformDisplayReturnedRows; q++) {  //now work through the returned results
    myString = String(platformDisplayTime[q]) + ' ' + String(platformDisplayArrDep[q]) + ' ' + String(platformDisplayDestination[q]);
    if (q < 2) {  //display can only show 2 results so doint send exessive transmissions
      myLayout.sendString(1000 + q, myString);
    }
    // Serial.print(myString);
    // Serial.print(" : ");
    // myString = String(platformDisplayArrDep[q]);  //arr dep
    // Serial.print(myString);
    // Serial.print(" : ");
    // Serial.print(platformDisplayPlatformNum[q]);  //uint16_t platform number 0 if none
    // Serial.print(" : ");
    // myString = String(platformDisplayDestination[q]);  //destination
    // Serial.print(myString);
    // Serial.println("  ");
  }
  Serial.println("  ");
}


//results for platform 2
void plat2PlatformDisplayInfoResults() {
  uint16_t q;
  String myString;
  Serial.print("Platform 2 ");
  Serial.print("platformDisplayReturnedRows: ");
  Serial.println(platformDisplayReturnedRows);  //number of results
  Serial.print("nextTrainTime: ");
  Serial.println(nextTrainTime);
  platform2DisplayNextTrain = nextTrainTime;           //set here to prevent excessive triggering
  for (q = 0; q < platformDisplayReturnedRows; q++) {  //now work through the returned results
    myString = String(platformDisplayTime[q]);         //time string
    Serial.print(myString);
    Serial.print(" : ");
    myString = String(platformDisplayArrDep[q]);  //arr dep
    Serial.print(myString);
    Serial.print(" : ");
    Serial.print(platformDisplayPlatformNum[q]);  //uint16_t platform number 0 if none
    Serial.print(" : ");
    myString = String(platformDisplayDestination[q]);  //destination
    Serial.print(myString);
    Serial.println("  ");
  }
  Serial.println("  ");
}


//gets the results from sdOperatorsTimeTable call
void sdOperatorsTimeTableResults() {
  uint16_t q;
  String myString;
  Serial.print("opTimeTableReturnedRows: ");
  Serial.println(opTimeTableReturnedRows);  //number of results
  Serial.print("nextTrainTime: ");
  Serial.println(nextTrainTime);                   //number of results
  operatorsNextTrain = nextTrainTime;              //set here to prevent excessive triggering
  for (q = 0; q < opTimeTableReturnedRows; q++) {  //now work through the returned results
    myString = String(opTimeTableTime[q]);         //time string
    // Serial.print(myString);
    // Serial.print(" : ");
    // myString = String(opTimeTableArrDep[q]);  //arr dep
    // Serial.print(myString);
    // Serial.print(" : ");
    myLayout.sendString(100 + q, String(opTimeTableTime[q]) + ' ' + String(opTimeTableArrDep[q]));
    //myString = String(opTimeTableTrainType[q]);  //train type
    myLayout.sendString(110 + q, String(opTimeTableTrainType[q]));
    // Serial.print(myString);
    // Serial.print(" : ");
    // Serial.print(opTimeTablePlatformNum[q]);  //uint16_t platform number 0 if none
    // Serial.print(" : ");
    //myString = String(opTimeTableDestination[q]);  //destination
    if (q < 2) {  //only required for 1st 2 results on operator screen
      myLayout.sendString(120 + q, String(opTimeTableDestination[q]));
      // Serial.print(myString);
      // Serial.print(" : ");
      // myString = String(opTimeTableNotes[q]);  //Notes
      myLayout.sendString(130 + q, String(opTimeTableNotes[q]));
      //Serial.print(myString);
      //Serial.println(' ');
    }
  }
}

//This function controls what files are to be read and when
void processSDCard(byte clockHour, byte clockMin, byte clockSecond, byte clockDay) {
  //this function is called once per layout second so this function checks it's the next minute
  checkTime[0] = (clockHour * 100) + clockMin;  //required so evrything knows what time to start processing from as
  if (checkTime[0] != checkTime[1]) {           //next minute
    checkTime[1] = checkTime[0];
    //startupSdAccessoryCommands("AccessoryCommands.csv", checkTime[0], checkTime[0]);  //This will work through the time period between the two times and set aaccessories, runs once at start up
    sdAccessoryCommands("AccessoryCommands.csv");  //check for accessory commands every minute

    //if (checkTime[0] > operatorsNextTrain) {  //this if statement reduces the number of nowRail text transmissions
    sdOperatorsTimeTable("TimeTable.csv");      //called every minute
    sdOperatorsTimeTableResults();              //process the results
                                                //}
                                                //if (checkTime[0] > platformDisplayNextTrain) {
    sdPlatformDisplayInfo("TimeTable.csv", 0);  //called every minute All platforms
    allPlatformDisplayInfoResults();            //processes results for all platforms
                                                // }
                                                // if (checkTime[0] > platform1DisplayNextTrain) {
    sdPlatformDisplayInfo("TimeTable.csv", 1);  //called every minute platform 1
    plat1PlatformDisplayInfoResults();
    // }
    // if (checkTime[0] > platform2DisplayNextTrain) {
    sdPlatformDisplayInfo("TimeTable.csv", 2);  //called every minute platform 2
    plat2PlatformDisplayInfoResults();
    // }
  }
}














//Do not edit below here unless you understand the code



//gets platform display info
void sdPlatformDisplayInfo(String ttFile, uint16_t platform) {
  uint16_t q;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;
  byte timeTableNumRows;
  byte commaCount;
  uint8_t timebytes[4];
  char timeChars[4] = { '0', '0', '0', '0' };
  uint8_t timeBytesLength;
  char arrDepChars[10];  //just in case longer than arr or dep
  uint8_t arrDepCharsLength;
  char trainTypeChars[100];  //kept as same length as original string
  uint8_t trainTypeCharsLength;
  uint8_t platformTTLocationBytes[5];
  uint8_t platformTTLocationBytesLength;
  uint16_t ttPlatformLocationNum;
  char platformTTChars[100];  //kept as same length as original string
  uint8_t platformTTCharsLength;
  char ttNotesChars[100];  //kept as same length as original string
  uint8_t ttNotesCharsLength;
  uint16_t lineTime;

  File file = SD.open("/" + ttFile);
  if (!file) {
    Serial.println("Failed to open " + ttFile + " for reading");
    return;
  }
  while (file.available()) {
    nextChar = file.read();
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {        //printable character
      lineStringLength++;       //it's a real character
      if (nextChar != ',') {    //if it's not a comma
        switch (commaCount) {
          case 0:                                                         //time
            if (charByte > 47 && charByte < 58 && timeBytesLength < 4) {  //only deal with numeric values
              timebytes[timeBytesLength] = charByte - 48;                 //get numerica value
              timeChars[timeBytesLength] = nextChar;
              timeBytesLength++;
            }
            break;
          case 1:                          //arrive dep terminate
            if (arrDepCharsLength < 10) {  //only deal with numeric values
              arrDepChars[arrDepCharsLength] = nextChar;
              arrDepCharsLength++;
            }
            break;
          case 2:                              //Train Type
            if (trainTypeCharsLength < 100) {  //only deal with numeric values
              trainTypeChars[trainTypeCharsLength] = nextChar;
              trainTypeCharsLength++;
            }
            break;
          case 3:                                                                       //platform display location
            if (charByte > 47 && charByte < 58 && platformTTLocationBytesLength < 5) {  //don't overrun the arraysize
              platformTTLocationBytes[platformTTLocationBytesLength] = charByte - 48;
              platformTTLocationBytesLength++;
            }
            break;
          case 4:                               //Platform Announcement
            if (platformTTCharsLength < 100) {  //only deal with numeric values
              platformTTChars[platformTTCharsLength] = nextChar;
              platformTTCharsLength++;
            }
            break;
          case 5:                            //Notes
            if (ttNotesCharsLength < 100) {  //only deal with numeric values
              ttNotesChars[ttNotesCharsLength] = nextChar;
              ttNotesCharsLength++;
            }
            break;
          default:  //oops
            break;
        }
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {                                                   //end of line
      if (lineStringLength > 0) {                              //is it a valid line//is this greater than the current line?
        if (timeBytesLength > 0) {                             //do I have a time...if not misformed line
          if (timeTableNumRows < NUMPLATFORMDISPLAYRESULTS) {  //Do I already have all the results I need
            switch (timeBytesLength) {                         //get the time first
              case 1:
                lineTime = timebytes[0];
                break;
              case 2:
                lineTime = (timebytes[0] * 10) + timebytes[1];
                break;
              case 3:
                lineTime = (timebytes[0] * 100) + (timebytes[1] * 10) + timebytes[2];
                break;
              case 4:
                lineTime = (timebytes[0] * 1000) + (timebytes[1] * 100) + (timebytes[2] * 10) + timebytes[3];
                break;
              default:
                break;
            }
            if (lineTime >= checkTime[0]) {             //is this a line I should record
              switch (platformTTLocationBytesLength) {  //get the time first
                case 1:
                  ttPlatformLocationNum = platformTTLocationBytes[0];
                  break;
                case 2:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 10) + platformTTLocationBytes[1];
                  break;
                case 3:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 100) + (platformTTLocationBytes[1] * 10) + platformTTLocationBytes[2];
                  break;
                case 4:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 1000) + (platformTTLocationBytes[1] * 100) + (platformTTLocationBytes[2] * 10) + platformTTLocationBytes[3];
                  break;
                case 5:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 10000) + (platformTTLocationBytes[1] * 1000) + (platformTTLocationBytes[2] * 100) + (platformTTLocationBytes[3] * 10) + platformTTLocationBytes[4];
                  break;
                default:
                  ttPlatformLocationNum = 0;
                  break;
              }
              platformDisplayTime[timeTableNumRows][2] = ':';
              platformDisplayTime[timeTableNumRows][5] = '\0';  //null terminator
              if (lineTime < 10) {
                platformDisplayTime[timeTableNumRows][0] = '0';
                platformDisplayTime[timeTableNumRows][1] = '0';
                platformDisplayTime[timeTableNumRows][3] = '0';
                platformDisplayTime[timeTableNumRows][4] = timeChars[0];
              } else {
                if (lineTime < 100) {
                  platformDisplayTime[timeTableNumRows][0] = '0';
                  platformDisplayTime[timeTableNumRows][1] = '0';
                  platformDisplayTime[timeTableNumRows][3] = timeChars[0];
                  platformDisplayTime[timeTableNumRows][4] = timeChars[1];
                } else {
                  if (lineTime < 1000) {
                    platformDisplayTime[timeTableNumRows][0] = '0';
                    platformDisplayTime[timeTableNumRows][1] = timeChars[0];
                    platformDisplayTime[timeTableNumRows][3] = timeChars[1];
                    platformDisplayTime[timeTableNumRows][4] = timeChars[2];
                  } else {
                    platformDisplayTime[timeTableNumRows][0] = timeChars[0];
                    platformDisplayTime[timeTableNumRows][1] = timeChars[1];
                    platformDisplayTime[timeTableNumRows][3] = timeChars[2];
                    platformDisplayTime[timeTableNumRows][4] = timeChars[3];
                  }
                }
              }
              memcpy(platformDisplayArrDep[timeTableNumRows], arrDepChars, arrDepCharsLength);  //copy data across
              platformDisplayArrDep[timeTableNumRows][arrDepCharsLength] = '\0';                //null terminator
              platformDisplayPlatformNum[timeTableNumRows] = ttPlatformLocationNum;
              memcpy(platformDisplayDestination[timeTableNumRows], platformTTChars, platformTTCharsLength);  //copy data across
              platformDisplayDestination[timeTableNumRows][platformTTCharsLength] = '\0';                    //null terminator
              //this is how it's decided if this is a correct platformDisplay
              if (platform > 0) {  //specified platform number
                if (ttPlatformLocationNum == platform) {
                  timeTableNumRows++;
                }
              } else {
                if (ttPlatformLocationNum > 0) {  //lists all platform entries with a platform value != 0
                  timeTableNumRows++;
                }
              }
            }
          }  // end of if(timeTableNumRows
        }
      }
      commaCount = 0;
      lineStringLength = 0;  //reset for next line
      timeBytesLength = 0;   //reset for next line
      arrDepCharsLength = 0;
      trainTypeCharsLength = 0;
      platformTTLocationBytesLength = 0;
      platformTTCharsLength = 0;
      ttNotesCharsLength = 0;
      ttPlatformLocationNum = 0;
      for (q = 0; q < 5; q++) {
        platformTTLocationBytes[q] = 0;  //reset for next line
      }
    }
  }
  Serial.println(" ");
  file.close();  //close the file
  platformDisplayReturnedRows = timeTableNumRows;
}

//timetable and platform displays
void sdOperatorsTimeTable(String ttFile) {
  uint16_t q;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;
  byte timeTableNumRows;
  byte commaCount;
  uint8_t timebytes[4];
  char timeChars[4] = { '0', '0', '0', '0' };
  uint8_t timeBytesLength;
  char arrDepChars[10];  //just in case longer than arr or dep
  uint8_t arrDepCharsLength;
  char trainTypeChars[100];  //kept as same length as original string
  uint8_t trainTypeCharsLength;
  uint8_t platformTTLocationBytes[5];
  uint8_t platformTTLocationBytesLength;
  uint16_t ttPlatformLocationNum;
  char platformTTChars[100];  //kept as same length as original string
  uint8_t platformTTCharsLength;
  char ttNotesChars[100];  //kept as same length as original string
  uint8_t ttNotesCharsLength;
  uint16_t lineTime;

  nextTrainTime = 0;  //reset to Zero

  File file = SD.open("/" + ttFile);
  if (!file) {
    Serial.println("Failed to open " + ttFile + " for reading");
    return;
  }
  while (file.available()) {
    nextChar = file.read();
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {        //printable character
      lineStringLength++;       //it's a real character
      if (nextChar != ',') {    //if it's not a comma
        switch (commaCount) {
          case 0:                                                         //time
            if (charByte > 47 && charByte < 58 && timeBytesLength < 4) {  //only deal with numeric values
              timebytes[timeBytesLength] = charByte - 48;                 //get numerica value
              timeChars[timeBytesLength] = nextChar;
              timeBytesLength++;
            }
            break;
          case 1:                          //arrive dep terminate
            if (arrDepCharsLength < 10) {  //only deal with numeric values
              arrDepChars[arrDepCharsLength] = nextChar;
              arrDepCharsLength++;
            }
            break;
          case 2:                              //Train Type
            if (trainTypeCharsLength < 100) {  //only deal with numeric values
              trainTypeChars[trainTypeCharsLength] = nextChar;
              trainTypeCharsLength++;
            }
            break;
          case 3:                                                                       //platform display location
            if (charByte > 47 && charByte < 58 && platformTTLocationBytesLength < 5) {  //don't overrun the arraysize
              platformTTLocationBytes[platformTTLocationBytesLength] = charByte - 48;
              platformTTLocationBytesLength++;
            }
            break;
          case 4:                               //Platform Announcement
            if (platformTTCharsLength < 100) {  //only deal with numeric values
              platformTTChars[platformTTCharsLength] = nextChar;
              platformTTCharsLength++;
            }
            break;
          case 5:                            //Notes
            if (ttNotesCharsLength < 100) {  //only deal with numeric values
              ttNotesChars[ttNotesCharsLength] = nextChar;
              ttNotesCharsLength++;
            }
            break;
          default:  //oops
            break;
        }
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {                                                     //end of line
      if (lineStringLength > 0) {                                //is it a valid line//is this greater than the current line?
        if (timeBytesLength > 0) {                               //do I have a time...if not misformed line
          if (timeTableNumRows < NUMOPERATORTIMETABLERESULTS) {  //Do I already have all the results I need
            switch (timeBytesLength) {                           //get the time first
              case 1:
                lineTime = timebytes[0];
                break;
              case 2:
                lineTime = (timebytes[0] * 10) + timebytes[1];
                break;
              case 3:
                lineTime = (timebytes[0] * 100) + (timebytes[1] * 10) + timebytes[2];
                break;
              case 4:
                lineTime = (timebytes[0] * 1000) + (timebytes[1] * 100) + (timebytes[2] * 10) + timebytes[3];
                break;
              default:
                break;
            }
            if (lineTime >= checkTime[0]) {  //is this a line I should record
              if (timeTableNumRows < 1) {
                nextTrainTime = lineTime;
              }
              switch (platformTTLocationBytesLength) {  //get the time first
                case 1:
                  ttPlatformLocationNum = platformTTLocationBytes[0];
                  break;
                case 2:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 10) + platformTTLocationBytes[1];
                  break;
                case 3:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 100) + (platformTTLocationBytes[1] * 10) + platformTTLocationBytes[2];
                  break;
                case 4:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 1000) + (platformTTLocationBytes[1] * 100) + (platformTTLocationBytes[2] * 10) + platformTTLocationBytes[3];
                  break;
                case 5:
                  ttPlatformLocationNum = (platformTTLocationBytes[0] * 10000) + (platformTTLocationBytes[1] * 1000) + (platformTTLocationBytes[2] * 100) + (platformTTLocationBytes[3] * 10) + platformTTLocationBytes[4];
                  break;
                default:
                  ttPlatformLocationNum = 0;
                  break;
              }

              opTimeTableTime[timeTableNumRows][2] = ':';
              opTimeTableTime[timeTableNumRows][5] = '\0';  //null terminator
              if (lineTime < 10) {
                opTimeTableTime[timeTableNumRows][0] = '0';
                opTimeTableTime[timeTableNumRows][1] = '0';
                opTimeTableTime[timeTableNumRows][3] = '0';
                opTimeTableTime[timeTableNumRows][4] = timeChars[0];

              } else {
                if (lineTime < 100) {
                  opTimeTableTime[timeTableNumRows][0] = '0';
                  opTimeTableTime[timeTableNumRows][1] = '0';
                  opTimeTableTime[timeTableNumRows][3] = timeChars[0];
                  opTimeTableTime[timeTableNumRows][4] = timeChars[1];
                } else {
                  if (lineTime < 1000) {
                    opTimeTableTime[timeTableNumRows][0] = '0';
                    opTimeTableTime[timeTableNumRows][1] = timeChars[0];
                    opTimeTableTime[timeTableNumRows][3] = timeChars[1];
                    opTimeTableTime[timeTableNumRows][4] = timeChars[2];
                  } else {
                    opTimeTableTime[timeTableNumRows][0] = timeChars[0];
                    opTimeTableTime[timeTableNumRows][1] = timeChars[1];
                    opTimeTableTime[timeTableNumRows][3] = timeChars[2];
                    opTimeTableTime[timeTableNumRows][4] = timeChars[3];
                  }
                }
              }
              memcpy(opTimeTableArrDep[timeTableNumRows], arrDepChars, arrDepCharsLength);           //copy data across
              opTimeTableArrDep[timeTableNumRows][arrDepCharsLength] = '\0';                         //null terminator
              memcpy(opTimeTableTrainType[timeTableNumRows], trainTypeChars, trainTypeCharsLength);  //copy data across
              opTimeTableTrainType[timeTableNumRows][trainTypeCharsLength] = '\0';                   //null terminator
              opTimeTablePlatformNum[timeTableNumRows] = ttPlatformLocationNum;
              memcpy(opTimeTableDestination[timeTableNumRows], platformTTChars, platformTTCharsLength);  //copy data across
              opTimeTableDestination[timeTableNumRows][platformTTCharsLength] = '\0';                    //null terminator

              memcpy(opTimeTableNotes[timeTableNumRows], ttNotesChars, ttNotesCharsLength);  //copy data across
              opTimeTableNotes[timeTableNumRows][ttNotesCharsLength] = '\0';                 //null terminator
              timeTableNumRows++;
            }
          }  // end of if(timeTableNumRows
        }
      }
      commaCount = 0;
      lineStringLength = 0;  //reset for next line
      timeBytesLength = 0;   //reset for next line
      arrDepCharsLength = 0;
      trainTypeCharsLength = 0;
      platformTTLocationBytesLength = 0;
      platformTTCharsLength = 0;
      ttNotesCharsLength = 0;
      ttPlatformLocationNum = 0;
      for (q = 0; q < 4; q++) {
        timebytes[q] = 0;  //reset for next line
        timeChars[q] = '0';
      }
      for (q = 0; q < 5; q++) {
        platformTTLocationBytes[q] = 0;  //reset for next line
      }
    }
  }
  file.close();  //close the file
  opTimeTableReturnedRows = timeTableNumRows;
}

//This checks for accessory commands that are due and if it finds one triggers it
void sdAccessoryCommands(String accFile) {
  uint16_t q;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;
  byte commaCount;
  uint8_t timebytes[4];
  uint8_t timeBytesLength;
  uint8_t AccBytes[5];
  uint8_t AccBytesLength;
  uint8_t dirBytes[3];
  byte dirBytesLength;
  char descChars[100];
  byte descCharsLength;

  uint16_t lineTime;
  uint16_t lineAccNum;
  uint8_t lineAccDir;

  File file = SD.open("/" + accFile);
  if (!file) {
    //Serial.println("Failed to open " + accFile + " for reading");
    return;
  }
  while (file.available()) {
    nextChar = file.read();
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {
      lineStringLength++;     //it's a real character
      if (nextChar != ',') {  //if it's not a comma
        switch (commaCount) {
          case 0:                                                         //time
            if (charByte > 47 && charByte < 58 && timeBytesLength < 4) {  //only deal with numeric values
              timebytes[timeBytesLength] = charByte - 48;                 //get numerica value

              //Serial.println("C: "+ timebytes[timeBytesLength]);
              timeBytesLength++;
            }
            break;
          case 1:                                                        //acc num
            if (charByte > 47 && charByte < 58 && AccBytesLength < 5) {  //only deal with numeric values
              AccBytes[AccBytesLength] = charByte - 48;                  //get numerica value
              AccBytesLength++;
            }
            break;
          case 2:                                                        //acc dir
            if (charByte > 47 && charByte < 58 && dirBytesLength < 3) {  //only deal with numeric values and value array size range
              dirBytes[dirBytesLength] = charByte - 48;                  //get numerica value
              dirBytesLength++;
            }
            break;
          case 3:                         //desc
            if (descCharsLength < 100) {  //don't overrun the arraysize
              descChars[descCharsLength] = nextChar;
              descCharsLength++;
            }
            break;
          default:  //oops
            break;
        }
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {                                                                    //end of line
      if (lineStringLength > 0) {                                               //was this a real line or a 2nd escape character...only do something if atual value
        if (timeBytesLength > 0 && AccBytesLength > 0 && dirBytesLength > 0) {  //check line has values
          switch (timeBytesLength) {
            case 1:
              lineTime = timebytes[0];
              break;
            case 2:
              lineTime = (timebytes[0] * 10) + timebytes[1];
              break;
            case 3:
              lineTime = (timebytes[0] * 100) + (timebytes[1] * 10) + timebytes[2];
              break;
            case 4:
              lineTime = (timebytes[0] * 1000) + (timebytes[1] * 100) + (timebytes[2] * 10) + timebytes[3];
              break;
            default:
              break;
          }
          if (lineTime == checkTime[0]) {
            switch (AccBytesLength) {
              case 1:
                lineAccNum = AccBytes[0];
                break;
              case 2:
                lineAccNum = (AccBytes[0] * 10) + AccBytes[1];
                break;
              case 3:
                lineAccNum = (AccBytes[0] * 100) + (AccBytes[1] * 10) + AccBytes[2];
                break;
              case 4:
                lineAccNum = (AccBytes[0] * 1000) + (AccBytes[1] * 100) + (AccBytes[2] * 10) + AccBytes[3];
                break;
              case 5:
                lineAccNum = (AccBytes[0] * 10000) + (AccBytes[1] * 1000) + (AccBytes[2] * 100) + (AccBytes[3] * 10) + AccBytes[4];
                break;
              default:
                break;
            }
            switch (dirBytesLength) {
              case 1:
                lineAccDir = dirBytes[0];
                break;
              case 2:
                lineAccDir = (dirBytes[0] * 10) + dirBytes[1];
                break;
              case 3:
                lineAccDir = (dirBytes[0] * 100) + (dirBytes[1] * 10) + dirBytes[2];
                break;
              default:
                break;
            }
            myLayout.sendAccessoryCommand(lineAccNum, lineAccDir, MESSRESPREQ);
            Serial.print(lineTime);
            Serial.print(" - ");
            Serial.print(lineAccNum);
            Serial.print(" - ");
            Serial.print(lineAccDir);
            Serial.print(" - ");
            for (q = 0; q < descCharsLength; q++) {
              Serial.print(descChars[q]);
            }
            Serial.println("  ");
          }
        }
      }
      commaCount = 0;
      lineStringLength = 0;  //reset for next line
      timeBytesLength = 0;   //reset for next line
      AccBytesLength = 0;    //reset for next line
      dirBytesLength = 0;    //reset for next line
      descCharsLength = 0;   //reset for next line
      for (q = 0; q < 4; q++) {
        timebytes[q] = 0;  //reset for next line
      }
      for (q = 0; q < 5; q++) {
        AccBytes[q] = 0;  //reset for next line
      }
      for (q = 0; q < 3; q++) {
        dirBytes[q] = 0;  //reset for next line
      }
    }
  }
  file.close();  //close the file
}

void listDir() {
  File root;

  root = SD.open("/");   //returns 1 if true
  Serial.println(root);  //returns 1 if true
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    Serial.println(entry.name());
    entry.close();
  }
  Serial.println(" ");
}


//V2
//crude method to turn String into uint32_t
uint32_t StringToUint32(String myNumber) {
  uint32_t myULong = 0;
  uint16_t q;
  String charString;
  uint16_t w;
  uint32_t myMultiplier;
  for (q = 0; q < myNumber.length(); q++) {
    myMultiplier = 1;
    for (w = 0; w < myNumber.length() - 1 - q; w++) {
      myMultiplier = myMultiplier * 10;
    }
    charString = myNumber.charAt(q);
    myULong = myULong + (charString.toInt() * myMultiplier);
  }
  return myULong;
}
uint16_t numSensorToAccs;                        //counter so I know How many items added.
uint32_t sensorToAccsArray[MAXSENSORTOACCS][4];  //data store array

void loadSensorToAccs() {
  uint16_t q;
  uint8_t commaCount;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;

  String inputSensorNum;
  String inputSensorInst;
  String outputAccNum;
  String outputInst;

  File file = SD.open("/sensorToAccCommands.csv");
  if (!file) {
    Serial.println("Failed to open sensorToAccCommands for reading");
    return;  //exit function
  }
  while (file.available()) {  //work through the file
    nextChar = file.read();
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {
      if (nextChar != ',') {  //if it's not a comma
        if (charByte > 47 && charByte < 58) {
          lineStringLength++;  //only interested in numeric characters
          switch (commaCount) {
            case 0:  //inputSensorNum
              inputSensorNum = inputSensorNum + nextChar;
              break;
            case 1:  //inputSensorInst
              inputSensorInst = inputSensorInst + nextChar;
              break;
            case 2:  //outputAccNum
              outputAccNum = outputAccNum + nextChar;
              break;
            case 3:  //outputInst
              outputInst = outputInst + nextChar;
              break;
            default:
              break;
          }
        }
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {
      if (lineStringLength > 0) {
        if (StringToUint32(inputSensorNum) > 0 && StringToUint32(inputSensorInst) > 0 && StringToUint32(outputAccNum) > 0 && numSensorToAccs < MAXSENSORTOACCS) {
          //Serial.println("Valid Row");
          //0 = SequenceTime, 1 = Trigger Time, 2 = Random, 3 = Acc Trigger Num,4 = acc trigger state, 5 =  nextTrigger, 6 = RTrigger,   7 = OutPut Acc Num, 8 = Output Dir
          sensorToAccsArray[numSensorToAccs][0] = StringToUint32(inputSensorNum);
          sensorToAccsArray[numSensorToAccs][1] = StringToUint32(inputSensorInst);
          sensorToAccsArray[numSensorToAccs][2] = StringToUint32(outputAccNum);
          sensorToAccsArray[numSensorToAccs][3] = StringToUint32(outputInst);
          // Serial.print(sensorToAccsArray[numSensorToAccs][0]);
          // Serial.print(" : ");
          // Serial.print(sensorToAccsArray[numSensorToAccs][1]);
          // Serial.print(" : ");
          // Serial.print(sensorToAccsArray[numSensorToAccs][2]);
          // Serial.print(" : ");
          // Serial.println(sensorToAccsArray[numSensorToAccs][3]);
          numSensorToAccs++;  //increment for next line
        }
      }
      //reset everything for next line
      commaCount = 0;
      lineStringLength = 0;
      inputSensorNum = "";
      inputSensorInst = "";
      outputAccNum = "";
      outputInst = "";
    }
  }
}

void sensorToAcc(uint16_t senNum, uint16_t senInst) {
  uint16_t q;
  uint8_t sendInstruction;
  // Serial.print("XXXXXXXXXXXXXX: ");
  // Serial.print(senNum);
  // Serial.print(" : ");
  // Serial.println(senInst);
  for (q = 0; q < numSensorToAccs; q++) {
    if (sensorToAccsArray[q][0] == senNum) {     //valid sensor number
      if (senInst >= sensorToAccsArray[q][1]) {  //trigger accessory
        myLayout.sendAccessoryCommand(sensorToAccsArray[q][2], sensorToAccsArray[q][3], MESSRESPREQ);
      } else {                              //trigger opposite
        if (sensorToAccsArray[q][3] > 0) {  //reverse accessory instruction
          sendInstruction = 0;
        } else {
          sendInstruction = 1;
        }
        myLayout.sendAccessoryCommand(sensorToAccsArray[q][2], sendInstruction, MESSRESPREQ);
      }
      myLayout.sensorProcessed();  //stop sensor being repeated
    }
  }
}

uint16_t numRepeatAccessory;                             //counter so I know How many items added.
uint32_t repeatAccessoryArray[MAXREPEATAUTOMATIONS][9];  //0 = SequenceTime, 1 = Trigger Time, 2 = Random, 3 = Acc Trigger Num,4 = acc trigger state, 5 =  nextTrigger, 6 = RTrigger,   7 = OutPut Acc Num, 8 = Output Dir

void loadRepeatAnimations() {  //puts repeat animation data into array
  uint16_t q;
  uint8_t commaCount;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;

  String sequenceTime;
  String triggerTime;
  String randomTime;
  String accTriggerNum;
  String outputAccNum;
  String outputDir;

  File file = SD.open("/repeatAutomation.csv");
  if (!file) {
    Serial.println("Failed to open repeatAutomation.csv for reading");
    return;
  }
  while (file.available()) {  //work through the file
    nextChar = file.read();
    // Serial.print(nextChar);     //Print file to Serial monitor
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {
      if (nextChar != ',') {  //if it's not a comma
        if (charByte > 47 && charByte < 58) {
          lineStringLength++;  //only interested in numeric characters
          switch (commaCount) {
            case 0:  //Automation sequence length
              sequenceTime = sequenceTime + nextChar;
              break;
            case 1:  //Trigger time in seqence time
              triggerTime = triggerTime + nextChar;
              break;
            case 2:  //Random Time
              randomTime = randomTime + nextChar;
              break;
            case 3:
              accTriggerNum = accTriggerNum + nextChar;
              break;
            case 4:
              outputAccNum = outputAccNum + nextChar;
              break;
            case 5:
              outputDir = outputDir + nextChar;
              break;
            default:
              break;
          }
        }
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {
      if (lineStringLength > 0) {
        if (StringToUint32(sequenceTime) > 0 && StringToUint32(outputAccNum) > 0 && numRepeatAccessory < MAXREPEATAUTOMATIONS) {  //modded on V 3
          //Serial.println("Valid Row");
          //0 = SequenceTime, 1 = Trigger Time, 2 = Random, 3 = Acc Trigger Num,4 = acc trigger state, 5 =  nextTrigger, 6 = RTrigger,   7 = OutPut Acc Num, 8 = Output Dir
          repeatAccessoryArray[numRepeatAccessory][0] = StringToUint32(sequenceTime);
          repeatAccessoryArray[numRepeatAccessory][1] = StringToUint32(triggerTime);
          repeatAccessoryArray[numRepeatAccessory][2] = StringToUint32(randomTime);
          repeatAccessoryArray[numRepeatAccessory][3] = StringToUint32(accTriggerNum);
          //repeatAccessoryArray[numRepeatAccessory][4] = 0;
          repeatAccessoryArray[numRepeatAccessory][5] = StringToUint32(triggerTime);
          if (repeatAccessoryArray[numRepeatAccessory][2] > 0) {  //does it need a random time
            repeatAccessoryArray[numRepeatAccessory][6] = StringToUint32(triggerTime) + random(repeatAccessoryArray[numRepeatAccessory][2]);
          }
          repeatAccessoryArray[numRepeatAccessory][7] = StringToUint32(outputAccNum);
          repeatAccessoryArray[numRepeatAccessory][8] = StringToUint32(outputDir);

          numRepeatAccessory++;  //increment for next line
        }
      }
      //reset everything for next line
      commaCount = 0;
      lineStringLength = 0;
      sequenceTime = "";
      triggerTime = "";
      randomTime = "";
      accTriggerNum = "";
      outputAccNum = "";
      outputDir = "";
    }
  }
}

//0 = SequenceTime,
//1 = Trigger Time,
//2 = Random,
//3 = Acc Trigger Num,
//4 = acc trigger state,
//5 =  nextTrigger,
//6 = RTrigger,
//7 = OutPut Acc Num, 8 = Output Dir
void repeatAccessoryCommands(uint32_t sdMillis) {
  uint16_t q;
  uint16_t w;
  uint8_t triggerThisAcc;
  for (q = 0; q < numRepeatAccessory; q++) {
    //does this item need to be triggered?
    //does it have an accessory trigger and if so has it been triggered?
    triggerThisAcc = 0;
    if (repeatAccessoryArray[q][3] < 1 || repeatAccessoryArray[q][4] > 0) {
      //now what triggers it...normal or random time?

      if (repeatAccessoryArray[q][2] > 0) {  //it's random time

        if (sdMillis >= repeatAccessoryArray[q][6]) {  //am I past the random trigger time?
                                                       // Serial.println(" Rand: ");
          triggerThisAcc = 1;
        }
      } else {                                         //normal time
        if (sdMillis >= repeatAccessoryArray[q][5]) {  //am I past the random trigger time?
                                                       // Serial.println(" Std: ");
          triggerThisAcc = 1;
        }
      }
      if (triggerThisAcc > 0) {  //trigger the accessory

        myLayout.sendAccessoryCommand(repeatAccessoryArray[q][7], repeatAccessoryArray[q][8], MESSRESPREQ);  //send out accessory command
        //now update it
        if (repeatAccessoryArray[q][2] > 0) {  //if it's a random
          repeatAccessoryArray[q][6] = repeatAccessoryArray[q][5] + repeatAccessoryArray[q][0] + random(repeatAccessoryArray[q][2]);
        }
        repeatAccessoryArray[q][5] = repeatAccessoryArray[q][5] + repeatAccessoryArray[q][0];
      }
    }
  }
}

//updates the repeatAccessoryArray when accessory commands are received
void updateRepeatAccessoryCommands(uint16_t accNum, uint16_t accInst, uint32_t accMillis) {
  uint16_t q;
  for (q = 0; q < numRepeatAccessory; q++) {     //loop through all the entries
    if (repeatAccessoryArray[q][3] == accNum) {  //if it's the right accessory
      if (accInst < 1) {                         //turn sequence off
        repeatAccessoryArray[q][4] = 0;          //turn off
      } else {                                   //turn sequence on
        repeatAccessoryArray[q][4] = 1;          //turn on
        //now set timings starting from accMillis
        if (repeatAccessoryArray[q][2] > 0) {  //if it's a random
          repeatAccessoryArray[q][6] = accMillis + repeatAccessoryArray[q][1] + random(repeatAccessoryArray[q][2]);
        }
        repeatAccessoryArray[q][5] = accMillis + repeatAccessoryArray[q][1];
      }
      myLayout.accProcessed(1);
    }
  }
}

//core process when sensor instructions received
void sdSensCommands(uint16_t senNum, uint16_t senInst) {
  sensorToAcc(senNum, senInst);
}

//core process when acc commands received
void sdAccCommands(uint16_t accNum, uint16_t accInst, uint32_t accMillis) {
  updateRepeatAccessoryCommands(accNum, accInst, accMillis);
}

//This system works through timed accessories between the two time period and triggers the accessories very quickly.
uint8_t startupSdAccessoryCommandsMode;
String startupAccCommandsFileName;
uint16_t startupAccCommandsStartTime;
uint16_t startupAccCommandsEndTime;
uint16_t startupAccCommandsCurrentTime;
uint32_t startupAccCommandsLastMillis;

//This sets up the system ready for processing
void startupSdAccessoryCommands(String fileName, uint16_t checkStartTime, uint16_t checkEndTime) {
  // Serial.print("XXXXXXXXXXXXXXXXXXX ");
  // Serial.println(startupSdAccessoryCommandsMode);
  if (startupSdAccessoryCommandsMode == 0) {  //only runs once
    startupSdAccessoryCommandsMode = 1;
    startupAccCommandsFileName = fileName;
    startupAccCommandsStartTime = checkStartTime;
    startupAccCommandsEndTime = checkEndTime;
    startupAccCommandsCurrentTime = 0;
  }
}

//now it's set up I need to check through all accessories... 2 millissecond gaps to prevent overload if big file
void runStartupSdAccessoryCommands(uint32_t sdMillis) {
  if (startupSdAccessoryCommandsMode == 1) {            //only runs when in mode 1
    if (sdMillis - startupAccCommandsLastMillis > 0) {  // if a 2 milliseconds have past check next batch
      startupAccCommandsLastMillis = sdMillis;


      startupAccCommandsStartTime++;                    //set read time starting 1 greater than startup
      checkTime[0] = startupAccCommandsStartTime;       //set time to start reading file
      sdAccessoryCommands(startupAccCommandsFileName);  //read file and trigger any accessories

      if (startupAccCommandsStartTime > 2359) {  //if reached end of day
        startupAccCommandsStartTime = 0;         //start new day
      }
      if (startupAccCommandsStartTime == startupAccCommandsEndTime) {  //reached end of items that need to be checked
        startupSdAccessoryCommandsMode = 2;
      }
    }
  }
}

//core process driver millis timed events
void sdCoreProcess(uint32_t sdMillis) {
  repeatAccessoryCommands(sdMillis);
  runStartupSdAccessoryCommands(sdMillis);
}

//This function receives all time updates and is used to drive/check timed stuff
void sdTimeUpdate(byte clockHour, byte clockMin, byte clockSecond, byte clockDay) {
  processSDCard(clockHour, clockMin, clockSecond, clockDay);
}

//Loads loco data into array from SD Card
void loadLocoData() {
  uint16_t q;
  uint8_t commaCount;
  char nextChar;
  uint8_t charByte;
  uint16_t lineStringLength;
  String myDCCAddr;
  uint16_t myDCCAddrNum;
  String myLocoName;
  uint8_t locoCount;  //0-199 allowed

  File file = SD.open("/locoData.csv");
  if (!file) {
    Serial.println("Failed to open repeatAutomation.csv for reading");
    return;
  }
  while (file.available()) {  //work through the file
    nextChar = file.read();
    //Serial.print(nextChar);     //Print file to Serial monitor
    charByte = byte(nextChar);  //get the byte value
    if (charByte > 31) {
      if (nextChar != ',') {  //if it's not a comma
                              //if (charByte > 47 && charByte < 58) {
        lineStringLength++;   //only interested in numeric characters
        switch (commaCount) {
          case 0:  //myDCCAddr
            if (charByte > 47 && charByte < 58) {
              myDCCAddr = myDCCAddr + nextChar;
            }
            break;
          case 1:  //myLocoName
            myLocoName = myLocoName + nextChar;
            break;
          case 2:  //Notes so do nothing as not needed/stored in system

            break;

          default:
            break;
        }
        //}
      } else {
        commaCount++;  //if it's a comma increment commacount
      }
    } else {
      if (lineStringLength > 0) {
        myDCCAddrNum = myDCCAddr.toInt();
        Serial.println(myDCCAddrNum);
        Serial.println(myLocoName);
        if (myDCCAddrNum > 0 && myDCCAddrNum < 10000 && locoCount < 200) {  //valid dcc address and wihting array limits (200)
          myLayout.setLocoDCCAddress(locoCount, myDCCAddrNum);
          myLayout.setLocoName(locoCount, myLocoName);
          locoCount++;
        }
      }
      //reset everything for next line
      commaCount = 0;
      lineStringLength = 0;
      myDCCAddr = "";
      myLocoName = "";
    }
  }
}

//sets up the SD Card and sends any error messages to Serial monitor
void sdsetup() {
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  listDir();  //displays any files in the root directory to the Serial monitor
  loadRepeatAnimations();
  loadSensorToAccs();
  loadLocoData();
}
