/*
    Name:       button.ino
    Created:    2020-09-14
    Author:     Kuni-YOshi
*/

#include <M5Stack.h>
// The setup() function runs once each time the micro-controller starts
void setup() {
  // init lcd, serial, but don't init sd card
  M5.begin(true, false, true);
  
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("Button example");
  M5.Lcd.setCursor(3, 35);
  M5.Lcd.println("Press button B for 700ms");
  M5.Lcd.println("to clear screen.");
  M5.Lcd.setTextColor(RED);

  pinMode(2, OUTPUT); 
  pinMode(5, OUTPUT); 
  digitalWrite(2, HIGH); 
  digitalWrite(5, HIGH); 

}

// Add the main program code into the continuous loop() function
void loop() {
  // update button state
  M5.update();M5.Lcd.setCursor(3, 35);
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (M5.BtnA.wasPressed()) {
    
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 2 LOW");
    digitalWrite(2, LOW); 
    M5.Speaker.beep(); //beep
  } else if (M5.BtnA.wasReleased()) {
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 2 HIGH");
    digitalWrite(2, HIGH); 
  } 

  else if (M5.BtnB.wasPressed()) {
    
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 5 LOW");
    digitalWrite(5, LOW); 
    M5.Speaker.beep(); //beep
  } else if (M5.BtnB.wasReleased()) {
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 5 HIGH");
    digitalWrite(5, HIGH); 
  } 
  
   else if (M5.BtnC.wasPressed()) {
    
    
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 25 LOW");
    digitalWrite(2, LOW); digitalWrite(5, LOW); 
    M5.Speaker.beep(); //beep
  } else if (M5.BtnC.wasReleased()) {
    M5.Lcd.clear(BLACK);M5.Lcd.println("digitalWrite 25 HIGH");
    digitalWrite(2, HIGH); digitalWrite(5, HIGH); 
  } 
}
