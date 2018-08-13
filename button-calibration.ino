#include <Wire.h> 
#include <Tone.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// Load Settings & Functions
#include "Game-Settings.h"
#include "LED-Functions.h"
#include "Tone-Functions.h"

// Load Game Types
#include "Game-Tournament.h"
#include "Game-Memory.h"
#include "Game-Reaction.h"
#include "Game-War.h"
#include "Game-Melody.h"

///////////////////// Setup ////////////////////////
void setup() {
  //Serial.begin(9600);
  // Init LCD
  lcd.init();
  lcd.backlight();
  // ONLY FIRST BOOT!!! - Save DEFAULT
   for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
    }
  lcd.clear();
  lcd.print("CLEAN OK!");
  delay(1000);
  // end FIRST BOOT!

  lcd.clear();
  lcd.print(F("    WELCOME!    "));
  lcd.setCursor(0,1);
  lcd.print(F("YOU ARE IN GAME!"));

  GameType = 1;
  
  GameRand = analogRead(RANDOM_PIN); // Select pseudo-random sequences

  // Init Buttons
  for (int i = 0; i < 4; i++ ) { pinMode(pin_leds[i], OUTPUT); }
  pinMode(BLACK_BUTTON, INPUT_PULLUP);
  pinMode(WHITE_BUTTON, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(WHITE_BUTTON), WhiteButtonPressed, RISING);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // sets analog pin for input 

  // Init Speakers
  player_1.begin(PIN_PLAYER_1);
  player_2.begin(PIN_PLAYER_2);

  introduzione();
/* Debug version print to LCD
  lcd.clear();
  lcd.print (__DATE__);
  lcd.setCursor(0,1);
  lcd.print (__TIME__);
  lcd.setCursor(10,1);
  lcd.print (GAME_VER);
  delay(2000);
*/
  CloseGame(); // Set Default Values
  ActiveTime = millis(); // Set Start Value for Global Timer
}

///////////////// Main Loop ///////////////////////////

void loop() {
  // Let's Get It Started
  lcd.clear();
  int test = analogRead(BUTTON_PIN);
  //lcd.print(ValueToButton(test));
  //lcd.setCursor(0,1);
  lcd.print (test);
  delay(300);
}

