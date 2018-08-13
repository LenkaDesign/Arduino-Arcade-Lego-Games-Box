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

  eeAddress = 0;
  EEPROM.get(eeAddress, GameSound); // Get Sound Settings
  eeAddress += sizeof(bool);
  EEPROM.get(eeAddress, InMemory);
  // end OTHER BOOT!
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
  switch (GameState) {
    case 0: 
      GameState = GameType;
      while(!ButtonStatus[0])
      {
        ClearButtonStatus();
        GetButtonStatus();     
        // BLACK Change Type
        if (ButtonStatus[1]) {
           ITEMBLOCK();
           lcd.clear();
           GameState++;
           if (GameState > NUMGAME(GameTitle)) { GameState = 1; }
           lcd.print(GameTitle[GameState-1]);
           lcd.setCursor(0,1);
           lcd.print(F("WHITE to PLAY"));
        }
      
      // Sound OFF
      if (ButtonStatus[4]) {
        GameSound = !GameSound;
        lcd.setCursor(0,1);
        if (GameSound) {lcd.print(F("Sound ON        "));}
          else {lcd.print(F("Sound OFF       "));}
        EEPROM.update(0, GameSound); // Save to Memory
        DAMAGEWARP();
        lcd.setCursor(0,1);
        lcd.print(F("WHITE to PLAY"));
      }

      // Fast Type Select
      if (ButtonStatus[2]) { GameState = 1; break;} // Memory Game
      if (ButtonStatus[3]) { GameState = 2; break;} // Reaction Game
      if (ButtonStatus[5]) { GameState = 4; break;} // Melody Game

     }
    break;
    case 1: // Memory game
      POWERUP();
      MemoryGame();
      DAMAGEWARP();
    break;
    case 2: // Reaction game
      POWERUP();
      ReactionGame();
      DAMAGEWARP();
    break;
    case 3: // Tournament game
      POWERUP();
      TournamentGame();
      DAMAGEWARP();
    break;
    case 4: // Melody Game
      POWERUP();
      MelodyGame();
      DAMAGEWARP();
    break;
    case 5: // Nuclear War Game
      POWERUP();
      WarGame();
      DAMAGEWARP();
    break;
  } //end GameState while
}

