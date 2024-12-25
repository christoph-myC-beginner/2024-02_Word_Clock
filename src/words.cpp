#include "arduino.h"
#include <FastLED.h>

//extern NUM_LEDS;
extern CRGB leds[108];
int rot = 72;
int gruen = 202;
int blau = 60;

//declaration vor LEDs
//97  E S K I S T L F Ü N F  107   ==> ES IST FÜNF
//96  Z E H N Z W A N Z I G   86   ==> ZEHN ZWANZIG
//75  D R E I V I E R T E L   85   ==> DREI|VIERTEL
//74  T G N A C H V O R J M   64   ==> NACH VOR
//53  H A L B Q Z W Ö L F P   63   ==> HALB ZWÖLF
//52  Z W E I N S I E B E N   42   ==> ZW|EI|N|S|IEBEN
//31  K D R E I R H F Ü N F   41   ==> DREI FÜNF
//30  E L F N E U N V I E R   20   ==> ELF NEUN VIER
// 9  W A C H T Z E H N R S   19   ==> ACHT ZEHN
// 8  B S E C H S F M U H R    1   ==> SECHS UHR

void W_ESIST() {
  leds[97] .setRGB( rot,gruen, blau); //E
  leds[98] .setRGB( rot,gruen, blau); //S
  leds[100].setRGB( rot,gruen, blau); //I
  leds[101].setRGB( rot,gruen, blau); //S
  leds[102].setRGB( rot,gruen, blau); //T
}

void W_UHR() {
  leds[2].setRGB( rot,gruen, blau); //u
  leds[1].setRGB( rot,gruen, blau); //H
  leds[0].setRGB( rot,gruen, blau); //R
}

void M_FUENF() {
  for (int i=104; i <= 107; i++){
    leds[i].setRGB( rot,gruen, blau);
  }
}

void M_ZEHN() {
  for (int i=93; i <= 96; i++){
    leds[i].setRGB( rot,gruen, blau);
  }
}

void M_ZWANZIG () {
  for (int i=86; i <= 92; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void M_VIERTEL () {
  for (int i=79; i <= 85; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void W_NACH () {
  for (int i=69; i <= 72; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void W_VOR () {
  for (int i=66; i <= 68; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void M_HALB() {
  for (int i=53; i <= 56; i++){
    leds[i].setRGB( rot,gruen, blau);  
  }
}

void H_ZWOELF(){
  for (int i=58; i <= 62; i++){
    leds[i].setRGB( rot,gruen, blau);   
  }
}

void H_ZWEI(){
  for (int i=49; i <= 52; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void H_EIN(){
  for (int i=48; i <= 50; i++){
    leds[i].setRGB( rot,gruen, blau);   
  }
}

void H_EINS(){
  for (int i=47; i <= 50; i++){
    leds[i].setRGB( rot,gruen, blau);   
  }
}

void H_SIEBEN() {
  for (int i=42; i <= 47; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void H_DREI(){
  for (int i=32; i <= 35; i++){
    leds[i].setRGB( rot,gruen, blau);
  }
}

void H_FUENF(){
  for (int i=38; i <= 41; i++){
    leds[i].setRGB( rot,gruen, blau);
  }
}

void H_ELF () {
  for (int i=28; i <= 30; i++){
    leds[i].setRGB( rot,gruen, blau); 
  }
}

void H_NEUN (){
  for (int i=24; i <= 27; i++){
    leds[i].setRGB( rot,gruen, blau);  
  }
}

void H_VIER (){  
  for (int i=20; i <= 23; i++){
    leds[i].setRGB( rot,gruen, blau);   
  }
}

void H_ACHT (){
  for (int i=10; i <= 13; i++){
    leds[i].setRGB( rot,gruen, blau);  
  }
}

void H_ZEHN () {
  for (int i=14; i <= 17; i++){
    leds[i].setRGB( rot,gruen, blau);  
  }
}

void H_SECHS(){
  for (int i=3; i <= 7; i++){
    leds[i].setRGB( rot,gruen, blau);  
  }
}