int SaveGame = 0;

void MemoryGame () {
  GameType = 1;
  GameLevel = SaveGame;
  bool GameOver = false;
  byte CurrColor = 0;
  int interval = 650; 
  int a = 0;

  // Show BEST Score
    lcd.clear();
    lcd.print(F("BEST SCORE:"));
    lcd.setCursor(0,1);
    lcd.print(InMemory[0]);
    delay(2000);
  
  while (!GameOver) {
    ClearButtonStatus(); 
    PowerOffButtonLED();
    GameLevel++;

    lcd.clear();
    lcd.print(F("LEVEL #"));
    lcd.print(GameLevel);
    lcd.setCursor(0,1);
    
    // Change Spped via Interval
    if ((GameLevel % 5 == 0)&&(GameLevel <= 30)) {interval = interval-(GameLevel / 5) * 30;}
    if (GameLevel % 5 == 0) { SaveGame = GameLevel-1; }
    // 1 2 3 4 5 6 7 8 9 10

    lcd.print(F("REMEMBER MELODY!"));
        
    delay(1000);
    
    // Generate Melody
    randomSeed(GameRand); // Get Current Random Series
    for(int i=0; i < GameLevel; i++) {
      // Get Random Button
      CurrColor = random (0,4);
      PlayColor(pin_leds[CurrColor], 2);
      delay(interval);
      PowerOffButtonLED();
    }

    // Show Check Screen
    lcd.clear();
    ClearLine();
    lcd.setCursor(0,1);
    lcd.print(F("  PUSH & SING!  "));
    
    // Check Melody
    randomSeed(GameRand);
    for(int i=0; i < GameLevel; i++) {
      // Get Random Button
      CurrColor = random (0,4);
      
      // Show Current Status
      ClearLine();
      lcd.print(F("KEEP ON "));
      lcd.print(GameLevel-i);
      
      a = 0;
      while (!ButtonStatus[6])
        { 
          GetButtonStatus(); 
          // Assign Color
          if (ButtonStatus[2]) { a = LED_YELLOW;}     
          if (ButtonStatus[3]) { a = LED_GREEN;}       
          if (ButtonStatus[4]) { a = LED_RED;}         
          if (ButtonStatus[5]) { a = LED_BLUE;}  
        }
      // Play Color if Press Button
      if (a != 0) {PlayColor(a, 2); }
      PowerOffButtonLED();
      
      // Show Status Slogan
      lcd.setCursor(0,0);
      if ((a != 0)&&(a != pin_leds[CurrColor])) {GameOver = true; lcd.print(F(" YOU ARE WRONG! ")); }
      if ((a == pin_leds[CurrColor])&&(i == GameLevel-1)){ lcd.print(F("    PERFECT!    "));}
      if (ButtonStatus[1]) { GameOver = true; lcd.print(F("   GOOD LUCK!   "));}  // BLACK Button Rules

      // WHITE Button = Repeat Mode
      if (ButtonStatus[0]) { 
        GameLevel--; 
        lcd.print(F("   TRY AGAIN!   ")); 
        if (GameLevel < 0) {GameLevel = 0;} 
        break;
       }
      
      // Clear Button Status
      ClearButtonStatus(); 
      if (GameOver) { GameLevel--; break; } // Exit from Attempt if GameOver
    }
    delay(1500); // Get PAUSE
  }
  
  // Show Final Score
  lcd.clear();
  lcd.print(F("   GAME OVER   "));
  lcd.setCursor(0,1);
  // Save NEW record
  if (GameLevel > InMemory[0]) {
      InMemory[0] = GameLevel;
      lcd.print(F("NEW RECORD: "));
      eeAddress = sizeof(bool);
      EEPROM.put(eeAddress, InMemory[0]);
     } else {
      lcd.print(F("YOUR LEVEL: "));
    }
  lcd.print(GameLevel);
  GAMEOVER();  
  delay(3000);
  CloseGame(); 
  return;
}

