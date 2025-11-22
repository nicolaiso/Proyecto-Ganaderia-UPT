

#include <SparkFunSerialGraphicLCD.h> 
#include <SoftwareSerial.h>         

#define maxX 127
#define maxY 63


LCD LCD;

void setup() {
  delay(1200);       
  LCD.setHome();     
  LCD.clearScreen();    
  delay(10);


  LCD.setX((maxX / 2) - 18);
  LCD.setY(maxY / 2 - 4);
  LCD.printStr("UPT");
}

void loop() {
  