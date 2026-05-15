/*nowRailV1_9_3HandControllerV5_7
30/04/26

This tab was added to enable easier custom configuration
*/
//As standard controller works with centre off for speed control
#define CLOCKWISESPEEDINCREASE 1  //when uncommented controller will only increase speed (rev or fwd) by turning controller clockwise
//#define REVENCODER 1  //Uncomment to reverse encoder speed increase direction...encoder wired backwards

//This is required so that the controller knows what type of command it is sending to consists
#define CONSISTTYPE 0   //0 means individual commands sent out for each loco, works with DCC EX and NCE (with limits)
                        //1 means DCC EX EXrail consist control
//   NOTE - When using RECALL with a CONSIST set up,  pushing RECALL(to go to another locomotive) will immediately kill the consist power and move the consist to "unset" on the consist screen
//   NOTE - When using CONSIST with a NCE System, the Emergency Stop function will only stop the lead locomotive, if the consist was set up with the NowRail Controller.  The Push to Stop encoder will stop all locomotives
//   NOTE - When using CONSIST with a NCE System that is a programmed consist through NCE, the Emergency Stop will stop all locomotives within the consist.
    
                        

//allows you to customise button colours
#define BUTTONCOLOR TFT_YELLOW //buttons when ON
#define SPEEDFWDBGCOLOR TFT_GREEN //fwd direction background
#define SPEEDFWDTXTCOLOR TFT_BLACK //fwd direction text
#define SPEEDREVBGCOLOR TFT_RED //rev direction background
#define SPEEDREVTXTCOLOR TFT_WHITE //rev direction text
#define PWRCOLOR TFT_RED //power button colour

/*
Colours can be set using the predefined definitions below or using HEX colour values

#define SPEEDREVBGCOLOR TFT_RED

is the same as
#define SPEEDREVBGCOLOR 0xF800

Standard Colours available (from TFT_eSPI library)

TFT_BLACK       
TFT_NAVY       
TFT_DARKGREEN   
TFT_DARKCYAN    
TFT_MAROON      
TFT_PURPLE      
TFT_OLIVE       
TFT_LIGHTGREY   
TFT_DARKGREY   
TFT_BLUE        
TFT_GREEN       
TFT_CYAN        
TFT_RED         
TFT_MAGENTA     
TFT_YELLOW      
TFT_WHITE       
TFT_ORANGE      
TFT_GREENYELLOW 
TFT_PINK        
TFT_BROWN       
TFT_GOLD        
TFT_SILVER      
TFT_SKYBLUE     
TFT_VIOLET      

*/
//Main screen Functions... Max 2 characters
#define Func0 'L'
#define Func1 "SW"
#define Func2 "SQ"
#define Func3 "CO"
#define Func4 '4'
#define Func5 '5'
#define Func6 '6'
#define Func7 '7'
#define Func8 '8'
#define Func9 '9'


#define NONLATCHTIMER 500 //sets the non latching reset time
//Non latching function buttons
// #define FUNC0NONLATCH
// #define FUNC1NONLATCH
// #define FUNC2NONLATCH
// #define FUNC3NONLATCH
// #define FUNC4NONLATCH
// #define FUNC5NONLATCH
// #define FUNC6NONLATCH
// #define FUNC7NONLATCH
// #define FUNC8NONLATCH
// #define FUNC9NONLATCH
// #define FUNC10NONLATCH
// #define FUNC11NONLATCH
// #define FUNC12NONLATCH
// #define FUNC13NONLATCH
// #define FUNC14NONLATCH
// #define FUNC15NONLATCH
// #define FUNC16NONLATCH
// #define FUNC17NONLATCH
// #define FUNC18NONLATCH
// #define FUNC19NONLATCH
// #define FUNC20NONLATCH
// #define FUNC21NONLATCH
// #define FUNC22NONLATCH
// #define FUNC23NONLATCH
// #define FUNC24NONLATCH
// #define FUNC25NONLATCH
// #define FUNC26NONLATCH
// #define FUNC27NONLATCH
// #define FUNC28NONLATCH
// #define FUNC29NONLATCH


//screen 6 Consists... max 2 characters
#define LocoFWD 'F'
#define LocoREV 'R'
#define LastLocoFWD "LF"    //Loco at rear of consist
#define LastLocoREV "LR"    //Loco at rear of consist

//Screen 9 Accessory control...Max
#define ACCON "ON/Straight"    //accessory command
#define ACCOFF "OFF/Diverge"  //accessory command

//Screen Color Reversing 
// #define COLORINVERT  // If your screen coulors seem strange uncomment this line

#define SCREENBUTTONDEBOUNCE 350 //debounce timer for buttons

//Rob Weaver mods to improve NCE compatibility

//  ** RECALL set timer **
//  The user can set how long the RECALL timer is active before it sets the recall.  
//As user scrolls through the loco recall button the button goes Yellow. Once it goes black the current loco is selected
#define RECALLSETTIMER 3000 //defines time in milliseconds before loco is selected as current loco

//if uncommented the Power screen will start as if power is on (NCE systems), otherwise will start up with "Syart" to send power command to DCC-EX
#define STARTFIRST 0  //  Makes the Screen 1 on start up with Start (DCC EX), Set to 1 for NCE as power is ON at start up

//Defines if Power button (bottom right) is power or direction change button
//works with clockwise only speed increase or anticlockwise reverse
#define DIRECTIONORPOWER 0  //0 = Power 1 = direction

//Accidental touch system means that the lower 6 buttons need to be double pressed using the standard debounce time
//#define ACCIDENTALTOUCHPROTECTION 0  //double touchincluding direction/power
//#define ACCIDENTALTOUCHPROTECTION 1  //double touch except direction/power
#define ACCIDENTALTIME 1000 //time period that 2nd press must be pressed within
#define ACCIDENTPUSHCOLOR TFT_PINK  //  Accidental push box timer color   // 5_4_1 controller add/change
#define ACCIDENTPUSHTEXTCOLOR TFT_BLACK //  Accidental push text color   // 5_4_1 controller add/change

