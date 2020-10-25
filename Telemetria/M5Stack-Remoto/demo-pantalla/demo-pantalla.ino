#include <Arduino.h>
#include <M5Stack.h>





void setup() {
  
    M5.begin(true, false, true);
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(70, 50);
    M5.Lcd.print("Amigus Labs");
    delay(500);
    M5.Lcd.setTextFont(3);
    M5.Lcd.setCursor(100, 150);
    M5.Lcd.print("Amigus Labs");
}

void loop() {
  


    delay(1000);

}
