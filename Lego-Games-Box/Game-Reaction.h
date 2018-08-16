void ReactionGame () {
  GameType = 2;
  bool GameOver = false;
  byte CurrColor = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;
  int GameScore = 0;
  int interval = 2000; 
  int a = 0;

  // Show BEST Score
  lcd.clear();
  lcd.print(F("BEST SCORE:"));
  lcd.setCursor(0,1);
  lcd.print(InMemory[1]);
  delay(2000);

  lcd.clear();
  lcd.print(F("Get Ready..."));
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print(F("START!"));

  
  while (!GameOver) {
    ClearButtonStatus();
    PowerOffButtonLED();
    
    // Get Random Button
    CurrColor = random (0,4);
    // and Play Color
    if (GameScore > 150) {PlayColor(pin_leds[CurrColor], 1);}
        else { PlayColor(pin_leds[CurrColor], 2); }
    SetButtonLED(pin_leds[CurrColor]);   
    currentMillis = previousMillis = millis();
        
    // Waiting User Action
    a = 0;
    lcd.setCursor(0,0);
    while (currentMillis - previousMillis <= interval)
      {
        SetButtonLED(pin_leds[CurrColor]);
        // Fill Line
        if ((currentMillis - previousMillis) % (interval/16) == 0) lcd.print((char)255);
        
        if (a == 0) { // Get Button Status
          GetButtonStatus();
          if (ButtonStatus[2]) { a = LED_YELLOW;}     
          if (ButtonStatus[3]) { a = LED_GREEN;}       
          if (ButtonStatus[4]) { a = LED_RED;}         
          if (ButtonStatus[5]) { a = LED_BLUE;}  
          if (a != 0) {break;}
          }
        currentMillis = millis();
      }
    lcd.clear();  
    // Show Status Slogan
    if ((a != 0)&&(a != pin_leds[CurrColor])) {GameOver = true; lcd.print(F(" YOU ARE WRONG! "));}
    if (a == pin_leds[CurrColor]){ GameScore++; lcd.print(F("   EXCELLENT!   "));}
    if (ButtonStatus[1]) { GameOver = true; lcd.print(F("   GOOD LUCK!   "));}  // BLACK Button Rules
        else if (a == 0) {GameOver = true; lcd.print(F(" YOU ARE LOOSE! "));}
    
    
    // Change Level
    switch (GameScore)
      {
        case 20:  interval = interval-100; lcd.clear();lcd.print(F("   !LEVEL UP!   ")); break;
        case 50:  interval = interval-100; lcd.clear();lcd.print(F("  !NEXT LEVEL!  ")); break;
        case 100: interval = interval-50;  lcd.clear();lcd.print(F("   IT'S 100!!   ")); break;
        case 150: interval = interval-50;  lcd.clear();lcd.print(F("REGARDS FOR YOU!")); break;
        case 200: interval = interval-50;  lcd.clear();lcd.print(F("  !200 CLICKZ!  ")); break;
        case 250: interval = interval-50;  lcd.clear();lcd.print(F("  PROFi LEVEL!  ")); break;
        case 300: interval = interval-50;  lcd.clear();lcd.print(F("DON'T BREAKE ME!")); break;
        case 500: interval = interval-50;  lcd.clear();lcd.print(F(" YOU ARE CRAZY! ")); break;
      }
    
    //delay(interval); // Keep Calm and Eat TWIX
    // Show Current Score
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(F("YOUR SCORE: "));
    lcd.print(GameScore);
    
  } // end while Game

  // Show Final Score
  lcd.clear();
  lcd.print(F("   GAME OVER   "));
  lcd.setCursor(0,1);
  // Save NEW record
  if (GameScore > InMemory[1]) {
      InMemory[1] = GameScore;
      lcd.print(F("NEW RECORD:"));
      eeAddress = sizeof(bool) + sizeof(word);
      EEPROM.put(eeAddress, InMemory[1]);
     } else {
      if (GameScore > 9999) {lcd.print(F("YOUR SCORE:"));}
      else {lcd.print(F("FINAL SCORE:"));}
    }
  // XXXX
  if (GameScore < 999) { lcd.print(F(" ")); }
  lcd.print(GameScore);
  GAMEOVER();  
  delay(3000);
  CloseGame(); 
  return;
}

