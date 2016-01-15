/*  
 *  Use an interrupt to calcuate RPM from the rising edge.
 *  Use analog input to read throttle position.
 *  Display on 16x2 LCD
 *  Author: Thomas Crossman
 *  Version: 1.1
 *  TPS voltage range changed to 0.712v to 3.91v
 *  LCD pins: https://www.arduino.cc/en/uploads/Tutorial/LCD_Base_bb_Schem.png
 */

#include <LiquidCrystal.h>

// Set the minimum and maximum TPS voltages here:

#define minTPSvolt 0.712                //voltage when tps = 0%
#define maxTPSvolt 3.91                 //voltage when tps = 100%

/*
 * LCD Pinout (LCD left to right increasing)
 * LCD------>1   2   3   4   5   6   7   8   9   10  11  12  13  14  15     16  
 * Arduino-->GND VCC GND 8   GND 9   -   -   -   -   10  11  12  13  220ohm GND
 */
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  //Setup lcd display see arduino.cc/lcd

const int RPMpin = 2;                   //incoming interrrupt on pin 2
const int TPSin = A0;                   //analog in for Throttle position
unsigned long timer,timerOld,timerDiff; //variable for measuring time between interrupts

//function declerations
void displayRPM(long int RPM);
void displayTPS(long int TPS);

void setup() {
  lcd.begin(16,2);                      //setup the 16x2 lcd
  timerOld = 0;                         //initilise to 0      
  pinMode(RPMpin,INPUT);                //set pin 2 as input
  attachInterrupt(digitalPinToInterrupt(RPMpin), findTime, RISING); // setupt interupt
  lcd.print("Hello Tom");               //welcome message
  lcd.setCursor(0,1);
  lcd.print("Have a nice ride");
  delay(2000);                          //delay before start
  lcd.setCursor(0,0);                   //reset lcd cursor position
  lcd.print("RPM     TPS");
}

void loop() {
  long int RPM,TPS;
  float TPSread,minTPS,maxTPS;
  minTPS = minTPSvolt/5*1024;           //convert min voltage to 0 - 1024
  maxTPS = maxTPSvolt/5*1024;           //convert max voltage to 0 - 1024
  TPSread = analogRead(TPSin);          //read voltage on pin 2  
  TPS = ((TPSread - minTPS)/(maxTPS - minTPS))*100; //convert TPS reading to percentage
  RPM = (1000000.00/timerDiff)*60;      //convert pulse time to RPM
  displayRPM(RPM);                      
  displayTPS(TPS);
  delay(250);                           //delay 100ms to make readable
}

//interrupt service routine to measure time between incoming RPM signals
void findTime (){
  timer = micros();                     //read process time
  timerDiff = (timer - timerOld);       //compare to previous interrupt
  timerOld = timer;                     //new time becomes oldtime
}

//display RPM
void displayRPM(long int RPM){
  lcd.setCursor(0,1);                   //RPM postiton is row 2 column 0
  lcd.print("        ");                //clear previous value
  lcd.setCursor(0,1);
  lcd.print(RPM);                       //write new value
}

//display throttle position
void displayTPS(long int TPS){
  lcd.setCursor(8,1);                   //tps starts at row 2 column 8
  lcd.print("        ");                //clear previous value
  lcd.setCursor(8,1);
  lcd.print(TPS);                       //write new value
  lcd.print("%");                       //add a percent symbol
}

