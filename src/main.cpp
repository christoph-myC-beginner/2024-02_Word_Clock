/*2024-02 Ansaetze fuer eine Wordclock. DS3231 als RTC. SD1306 0,96" Display für Uhr / Menue*/

#include <Arduino.h>

#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiWire display;

#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <time.h> 
#include <TimeLib.h>         //http://www.arduino.cc/playground/Code/Time  
DS3232RTC myRTC;

#include <FastLED.h>
#define NUM_LEDS 108
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 12
//#define CLOCK_PIN 13
// Define the array of leds
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  30

#include "OneButton.h"
// Setup a new OneButton on   
OneButton button1(9, true); //button push
OneButton button2(8, true); //button up
OneButton button3(7, true); //button down
OneButton button4(6, true); //button left
OneButton button5(5, true); //button right

/*
     GPIO  0 -
     GPIO  1 -
     GPIO  2 - button up
     GPIO  3 - button down
     GPIO  4 - button left
     GPIO  5 - button right
     GPIO  6 - button push (mid)
     GPIO 12 -
     GPIO 13 -
     GPIO 14 -
     GPIO 15 -
     GPIO 16 -
     GPIO A0 -
*/

#include "words.h"

#include "lmenu.h"
//Menue wichtige Eintraege 
menu a1,a2,a3,a4,a1a1,a1a2,a1a3,a2a1,a2a2,a2a3; //Menues entsprechend des struct definieren
//Menue-Eintraege hier vornehmen
static char* a1text= "Stelle Uhr";
static char* a2text= "Stelle Datum";
static char* a3text= "Setze Uhr";
static char* a4text= "Zeige RTC";
static char* a1a1text= "A1A1: Stelle Stunde";
static char* a1a2text= "A1A2: Stelle Minute";
static char* a1a3text= "A1A3: Stelle Sekunde";
static char* a2a1text= "A2A1: Stelle Jahr";
static char* a2a2text= "A2A2: Stelle Monat";
static char* a2a3text= "A2A3: Stelle Tag v Monat";

// put global Variables here
int vorZurueckB; //Variable die Mittels Knopf verÃ¤ndert wird; muss manuell zurÃ¼ck gesetzt werden
int aufAbB;
int setValFlag = 0;  //Variable fÃ¼r Eingabe einer Variablen

bool enterMenu = 0;

bool keepSetFlag;
bool keepNowFlag;
unsigned long keepMillis;
double lastMillis;

//SECONDS
int internalSeconds = 10;
int multiplierSecond = 1;
char* secondUnit = "s";
//MINUTES
int internalMinutes = 12;
int multiplierMinutes = 1;
char* minutesUnit = "min";
//HOURS
int internalHours = 19;
int multiplierHours = -1;
char* hoursUnit = "h";
//YEAR
int internalYear = 2024;
int multiplierYear = 1;
char* yearsUnit = "a";
//MONTH
int internalMonth = 12;
int multiplierMonth = 1;
char* monthsUnit = "month";
//DAY OF MONTH
int internalDays = 24;
int multiplierDays = 1;
char* daysUnit = "day of month";

int testHour = 11;
int showHour ;
int testMinute = 0;
int lastMinute ;

char *woTag[] = {"Null", "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

time_t nowRTC;
tmElements_t tmRTC;

//Farbe 
//int rot = 0;
//int gruen = 153;
//int blau = 153;

// put function declarations here:

//declaration of functions

//button1 (button push)
void click1();
void doubleclick1();
void longPressStart1();
void longPress1();
void longPressStop1();
//button2 (button up)
void click2();
void doubleclick2();
//button3 (button down)
void click3();
//button4 (button left)
void click4();
//button5 (button right)
void click5();

int keepFlagForTime(int keepTime, int flag);

//declaration menu functions
void a1action();
void a2action();
void a3action();
void a4action();
void a1a1action();
void a1a2action();
void a1a3action();
void a2a1action();
void a2a2action();
void a2a3action();
void link_menu();

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Setup of display SSD1306Ascii
  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
  display.clear();
  display.setFont(Callibri11);
  display.setCursor(8, 0);
  display.println("Last Change: ");
  display.setCursor(8, 18);
  display.println(__DATE__);
  delay(200);
  display.clear();

  //Setup of WS2812 LEDs
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  //
  FastLED.setBrightness(  BRIGHTNESS ); 
  for (int i = 0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(20);
  }
  for (int i = (NUM_LEDS - 1); i>=0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(20);
  } 
  //Setup of RTC
  myRTC.begin();
  nowRTC = myRTC.get();
  setSyncProvider(myRTC.get);
  
  // link the button 1 functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);
  button2.attachClick(click2);
  button3.attachClick(click3);
  button4.attachClick(click4);
  button5.attachClick(click5);

  //Setup Menue
  //menu_lcdInit(&lcd);
  //menu_ssd1306_init(&display);
  //menu_ST7735_init(&tft);
  //menu_u8g2_init(&u8g2);
  menu_ssd1306Ascii_init(&display);
  link_menu();
  menu_init(&a1, &setValFlag);
  vorZurueckB = -1;
  //menu_print3();
  //menu_ssd1306AsciiPrint();
  //menu_u8g2Print();
  //enterMenu = 1;
  //ENDE Setup MENUE

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if ((millis() - lastMillis) > 330) {
    lastMillis = millis();
    //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    //testMinute = testMinute + 1;
    
    testMinute = minute();
    if (hour() > 12) {
      testHour = hour() - 12;
    }
    else {
      testHour = hour();
    }
    //if (testMinute > 59) {
    //  testMinute = 0;
    ////  lastMinute = -1;
    //  testHour = testHour + 1;
    //}
    //if (testHour > 12) {
    //  testHour = 1;
    //}
  }
  
  if (testMinute != lastMinute) {
    lastMinute = testMinute;
    
    for (int i = (NUM_LEDS - 1); i>=0; i--) {
        leds[i] = CRGB::Black;
    }
    
    //Anzeige der Uhrzeit
    W_ESIST();
    W_UHR();
    if (testMinute >= 0 && testMinute < 3) {
      showHour = testHour;
    }
    if (testMinute >= 3 && testMinute <  8) {
      M_FUENF();
      W_NACH();
      showHour = testHour;
    }
    if (testMinute >= 8 && testMinute < 13) {
      M_ZEHN();
      W_NACH();
      showHour = testHour;
    }
    if (testMinute >= 13 && testMinute < 18) {
      M_VIERTEL();
      W_NACH();
      showHour = testHour;
    }
    if (testMinute >= 18 && testMinute < 23) {
      M_ZWANZIG();
      W_NACH();
      showHour = testHour;
    }
    if (testMinute >= 23 && testMinute < 28) {
      M_FUENF();
      W_VOR();
      M_HALB();
      showHour = testHour + 1;
    }
    if (testMinute >= 28 && testMinute < 33) {
      M_HALB();
      showHour = testHour + 1; 
    }
    if (testMinute >= 33 && testMinute < 38) {
      M_FUENF();
      W_NACH();
      M_HALB();
    }
    if (testMinute >= 38 && testMinute < 43) {
      M_ZWANZIG();
      W_VOR();
      showHour = testHour + 1; 
    }
    if (testMinute >= 43 && testMinute < 48) {
      M_VIERTEL();
      W_VOR();
      showHour = testHour + 1; 
    }
    if (testMinute >= 48 && testMinute < 53) {
      M_ZEHN();
      W_VOR();
      showHour = testHour + 1; 
    }
    if (testMinute >= 53 && testMinute < 58) {
      M_FUENF();
      W_VOR();
      showHour = testHour + 1; 
    }
    if (testMinute >= 58) {
      showHour = testHour + 1; 
    }
    if (showHour == 13) {showHour = 1;}
    if (showHour == 1 && testMinute < 3) {H_EIN();}
    if (showHour == 1 && testMinute >=3) {H_EINS();}
    if (showHour == 1 && testMinute > 58) {H_EIN();}
    if (showHour == 2) {H_ZWEI();}
    if (showHour == 3) {H_DREI();}
    if (showHour == 4) {H_VIER();}
    if (showHour == 5) {H_FUENF();}
    if (showHour == 6) {H_SECHS();}
    if (showHour == 7) {H_SIEBEN();}
    if (showHour == 8) {H_ACHT();}
    if (showHour == 9) {H_NEUN();}
    if (showHour == 10) {H_ZEHN();}
    if (showHour == 11) {H_ELF();}
    if (showHour == 12) {H_ZWOELF();}

    FastLED.show();
  }

  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
  button5.tick();
  
  enterMenu = 1;
  if (enterMenu == 1) {
    workMenu(aufAbB, vorZurueckB);
    aufAbB = 0;
    vorZurueckB = 0;
  }
  
  if (keepSetFlag == 1) {
    display.clear();
    display.setFont(X11fixed7x14B);
    display.setCursor(0, 0);
    display.println(" ");
    display.setCursor(20, 0);
    display.print(internalYear);
    display.print(" - ");
    display.print(internalMonth);
    display.print(" - ");
    display.println(internalDays);
    
    display.setCursor(0, 2);
    display.println(" ");
    display.setCursor(20, 2);
    display.print(internalHours);
    display.print(" : ");
    display.print(internalMinutes);
    display.print(" : ");
    display.println(internalSeconds);
  }
  keepSetFlag = keepFlagForTime(8000, keepSetFlag);
  
  if (keepNowFlag == 1) {
    display.clear();
    display.setFont(X11fixed7x14B);
    display.setCursor(0, 0);
    display.println("                    ");
    display.setCursor(20, 0);
    display.print(year());
    display.print(" - ");
    display.print(month());
    display.print(" - ");
    display.println(day());
    
    display.setCursor(0, 2);
    display.println("                    ");
    display.setCursor(20, 2);
    display.print(hour());
    display.print(" : ");
    display.print(minute());
    display.print(" : ");
    display.println(second());
  }
  keepNowFlag = keepFlagForTime(8000, keepNowFlag);

  if (internalSeconds >= 60) {
    //internalMinutes = internalMinutes + 1;
    internalSeconds = 0;
    drawVarSet_ssd1306Ascii(internalSeconds);
  }
  if (internalSeconds < 0) {
    internalSeconds = 59;
    drawVarSet_ssd1306Ascii(internalSeconds);
  }
  if (internalMinutes >= 60) {
    //internalHours = internalHours + 1;
    internalMinutes = 0;
    drawVarSet_ssd1306Ascii(internalMinutes);
  }
  if (internalMinutes < 0) {
    internalMinutes = 59;
    drawVarSet_ssd1306Ascii(internalMinutes);
  }
  if (internalHours >= 24) {
    internalHours = 0;
    drawVarSet_ssd1306Ascii(internalHours);
  }
  if (internalHours < 0) {
    internalHours = 23;
    drawVarSet_ssd1306Ascii(internalHours);
  }
  if (internalYear < 2024) {
    internalYear = 2025;
    drawVarSet_ssd1306Ascii(internalYear);
  }
  if (internalMonth > 12) {
    internalMonth = 1;
    drawVarSet_ssd1306Ascii(internalMonth);
  }
  if (internalMonth < 1) {
    internalMonth = 12;
    drawVarSet_ssd1306Ascii(internalMonth);
  }
  if (internalDays > 31) {
    internalDays = 1;
    drawVarSet_ssd1306Ascii(internalDays);
  }
  if (internalDays< 1) {
    internalDays = 31;
    drawVarSet_ssd1306Ascii(internalDays);
  }
}
//END OF MAIN

// Push-Button
void click1() {
  //Serial.println("Button 1 click.");
  vorZurueckB = 1;
} // click1

// Push-Button doppelklick
void doubleclick1() {
  //Serial.println("Button 1 doubleclick.");
  vorZurueckB = -1;
} // doubleclick1

// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  //Serial.println("Button 1 longPress start");
} // longPressStart1

// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
  //Serial.println("Button 1 longPress...");
} // longPress1

// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  //Serial.println("Button 1 longPress stop");
} // longPressStop1

void click2() {
  aufAbB = -1;
} // click2

void click3() {
  aufAbB = 1;
} // click3

void click4() {
  vorZurueckB = -1;
} // click4

void click5() {
  vorZurueckB = 1;
} // click5

//////////////////////////////////////////
//Menue Funktionen bei Aufruf
//////////////////////////////////////////
void a1action(){ 
  
}
void a2action(){ 
  
}
void a3action(){
  tmRTC.Hour = internalHours;
  tmRTC.Minute = internalMinutes;
  tmRTC.Second = internalSeconds;
  tmRTC.Year = (internalYear-1970);
  tmRTC.Month = internalMonth;
  tmRTC.Day = internalDays;
  nowRTC = makeTime(tmRTC);
  myRTC.set(nowRTC);
  keepSetFlag = 1;
  keepMillis = millis();
  //delay(5000);
}
void a4action(){ 
  setSyncProvider(myRTC.get);
  keepNowFlag = 1;
  keepMillis = millis();
}
void a1a1action(){ 
  //Set Hour
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierHours, &internalHours, hoursUnit);
    drawVarSet_ssd1306Ascii(internalHours);
  }
}
void a1a2action(){ 
  //Set Minutes
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierMinutes, &internalMinutes, minutesUnit);
    drawVarSet_ssd1306Ascii(internalMinutes);
  }
}
void a1a3action(){ 
  //Set Seconds
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierSecond, &internalSeconds, secondUnit);
    //drawVarSet(internalSeconds);
    drawVarSet_ssd1306Ascii(internalSeconds);
  }
}
void a2a1action(){ 
  //Set Year
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierYear, &internalYear, yearsUnit);
    drawVarSet_ssd1306Ascii(internalYear);
  }
}
void a2a2action(){ 
  //Set Month
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierMonth, &internalMonth, monthsUnit);
    drawVarSet_ssd1306Ascii(internalMonth);
  }
}
void a2a3action(){ 
  //Set Day of Month
  if (setValFlag == 0) {
    setValFlag = 1;
    ptr_init(&multiplierDays, &internalDays, daysUnit);
    drawVarSet_ssd1306Ascii(internalDays);
  }
}

//LINK Menue
void link_menu(){
  //Serial.println("Debug Menu linked");
//Main Menu
  a1.text = a1text;
  a1.up = &a4;
  a1.down = &a2;
  a1.ok = &a1a1;
  a1.back = &a1;
  a1.function = a1action;

  a2.text = a2text;
  a2.up = &a1;
  a2.down = &a3;
  a2.ok = &a2a1;
  a2.back = &a2;
  a2.function = a2action;

  a3.text = a3text;
  a3.up = &a2;
  a3.down = &a4;
  a3.ok = NULL;
  a3.back = &a3;
  a3.function = a3action;

  a4.text = a4text;
  a4.up = &a3;
  a4.down = &a1;
  a4.ok = NULL;
  a4.back = &a4;
  a4.function = a4action;

//Submenu
  a1a1.text = a1a1text;
  a1a1.up = &a1a3;
  a1a1.down = &a1a2;
  a1a1.ok = NULL;
  a1a1.back = &a1;
  a1a1.function = a1a1action;

  a1a2.text = a1a2text;
  a1a2.up = &a1a1;
  a1a2.down = &a1a3;
  a1a2.ok = NULL;
  a1a2.back = &a1;
  a1a2.function = a1a2action;

  a1a3.text = a1a3text;
  a1a3.up = &a1a2;
  a1a3.down = &a1a1;
  a1a3.ok = NULL;
  a1a3.back = &a1;
  a1a3.function = a1a3action;

  a2a1.text = a2a1text;
  a2a1.up = &a2a3;
  a2a1.down = &a2a2;
  a2a1.ok = NULL;
  a2a1.back = &a2;
  a2a1.function = a2a1action;

  a2a2.text = a2a2text;
  a2a2.up = &a2a1;
  a2a2.down = &a2a3;
  a2a2.ok = NULL;
  a2a2.back = &a2;
  a2a2.function = a2a2action;

  a2a3.text = a2a3text;
  a2a3.up = &a2a2;
  a2a3.down = &a2a1;
  a2a3.ok = NULL;
  a2a3.back = &a2;
  a2a3.function = a2a3action;
}

int keepFlagForTime(int keepTime, int flag) {
  if (keepTime == 0) {
    return flag;
  }
  else {
    if (millis()-keepMillis < keepTime) {
      return flag;
    }
    else {
      //menu_ssd1306AsciiPrint();
      return 0;
    }
  }
}

//End of File