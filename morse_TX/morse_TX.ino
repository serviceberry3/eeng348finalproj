#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
  
#include "state_mach.h"
#include "welcome.h"
#include "console.h"
#include "hypeglyphs.h"

state curr_state = INIT;

LiquidCrystal_I2C lcd(0x27,20,4);

//user requests
int req;

void setup() {
  Serial.begin(9600);

  //button 1 (for the morse)
  pinMode(push1, INPUT);

  pinMode(push2, INPUT);
 
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  digitalWrite(A3, HIGH);
  digitalWrite(A2, LOW);

  setup_periphs();
  lcd.init(); 
  lcd.backlight();
  lcd.createChar(0, cr_glyph);

  Serial.println("Setup done");

  interrupts();
  attachInterrupt(digitalPinToInterrupt(push2), onEnterButtonPressed, RISING);  
}

void loop() {
   switch (curr_state) {
     case INIT:
        req = welcome_user(lcd);
        break;
      case TYPING:
        open_console(lcd);
        break;
    }
}

void onEnterButtonPressed() {
  if (curr_state == INIT) {
    curr_state = TYPING;
  }
}
