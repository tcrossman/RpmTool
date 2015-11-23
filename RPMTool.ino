#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int switchPin = 6;

void setup() {
  lcd.begin(16,2);
  pinMode(switchPin,INPUT);
  lcd.print("Hello");
  lcd.cursor(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
