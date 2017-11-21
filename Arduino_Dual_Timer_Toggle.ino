/* LED ~ Arduino Pin Connection

  DMD Pin - GPIO #
  ================
  From Top Right (Notch Side)
  1  A      10
  2  B      12
  3   --Leave Open--
  4  CLK    13
  5  SCK    08
  6  R      11
  7   --Leave Open--
  8   --Leave Open--
  ____________________
  From Top Left (Plain Side)
  1  NOE    09
  2  GND    GND

  Middle Pin of Toggle Switch -> Pin4 (Attach a 1K Resistor)
  Other 2 pin of Toggle Switch -> VCC and GND
*/

#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Bold_18.h>
//#include <fonts/Cambria_Bold_16.h>
#define SwitchPin 4

bool lastState = LOW, buttonState = LOW;
unsigned long lastDebounceTime = 0, startTime = 0;
int counter = 60, secs = 7;//,centisec=0;hrs = 0, mins

SoftDMD dmd(3, 1, 9, 10, 12, 8, 13, 11);

void setup() {// 
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(Arial_Bold_18);//Cambria_Bold_16
  dmd.begin();
  dmd.clearScreen();
}

void cleardigit(int pos) {
  dmd.drawFilledBox(pos, 0, pos + 9, 15, GRAPHICS_OFF);
}

void displayCounter() {
  if (millis() - startTime > 1000) {
    startTime = millis();    //dmd.clearScreen();
    if (counter == 60) {
     /* if (secs == 0) {
        dmd.clearScreen();
      }      else {*/
      dmd.drawString(5, 0, "0");
      dmd.drawString(17, 0, "1"); 
        dmd.drawString(31, -1, ":");
        dmd.drawString(39, 0, "0");
        dmd.drawString(50, 0, "0");
        dmd.drawString(63, -1, ":");
        dmd.drawString(71, 0, "0");
        dmd.drawString(82, 0, "0");
       // dmd.drawString(0, 0, "Starts in ");//      counter--;
       // dmd.drawFilledBox(85, 0, 95, 15, GRAPHICS_OFF);
       // dmd.drawString(85, 0, String(secs));      }
    }
    else {
      dmd.drawString(5, 0, "0");
      dmd.drawString(17, 0, "0");      
      dmd.drawString(31, -1, ":");

      dmd.drawString(63, -1, ":");
      if (secs < 10) {
        dmd.drawString(71, 0, "0");
        cleardigit(82);
        dmd.drawString(82, 0, String(secs));
      }
      else {
        if (secs % 10 == 9) {
         cleardigit(71);// dmd.drawFilledBox(72, 0, 81, 15, GRAPHICS_OFF);//
          dmd.drawString(71, 0, String(secs / 10));
          Serial.println("Clearing 72");
      if (counter < 10) {
        dmd.drawString(39, 0, "0");
        cleardigit(50);
        dmd.drawString(50, 0, String(counter));
      }
      else {
        if (counter % 10 == 9) {
          cleardigit(39);//dmd.drawFilledBox(40, 0, 49, 15, GRAPHICS_OFF);//
          dmd.drawString(39, 0, String(counter / 10));
        }
        cleardigit(50);
        dmd.drawString(50, 0, String(counter % 10));
      }
        }
        cleardigit(82);
        dmd.drawString(82, 0, String(secs % 10));
      }
    }
    secs--;
    if (secs < 0) {
      counter--;
      secs = 59;
    }
  }
}

void loop() {
  bool reading = digitalRead(SwitchPin);

  if (reading != lastState) //{    //Serial.println("hello");
    lastDebounceTime = millis();//  }

  if ((millis() - lastDebounceTime) > 100) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {//        Serial.write("Count Down 60");
        counter = 60;
        secs = 5;
        dmd.clearScreen();
      }
      else {//        Serial.write("Count Down 2");
        counter = 2;
        secs = 0; //  stopshow = true;
        dmd.clearScreen();   //delay(1);    dmd.drawString(0, 1, "Wait");        
        dmd.drawString(5, 0, "0");
        dmd.drawString(17, 0, "0"); 
        dmd.drawString(31, -1, ":");
        dmd.drawString(39, 0, "0");
        dmd.drawString(50, 0, "2");
        dmd.drawString(63, -1, ":");
        dmd.drawString(71, 0, "0");
        dmd.drawString(82, 0, "0");
      }
    }
  }
  lastState = reading;

  if (counter <= 60 && counter >= 0)
    displayCounter();
}

