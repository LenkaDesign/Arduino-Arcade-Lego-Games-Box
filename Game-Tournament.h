void TournamentGame(){
    GameType = 3;
    GameLevel = 0;
    int GameRate[4] = {0,0,0,0};
    while (GameLevel < 5) {
      PowerOffButtonLED();
      ClearButtonStatus();
      GameLevel++;
      
      // Init state
      lcd.clear();
      PowerOffButtonLED();
      lcd.print(F("Who is First?"));
      lcd.setCursor(0,1);
      if (GameLevel == 5) { lcd.print(F("Final Lap...")); }
        else { lcd.print(F("Get Ready...")); }
      delay(random(1000, 2000)); // random delay 
      lcd.clear();
      lcd.print(F("!!PUSH ME DOWN!!"));
      lcd.setCursor(0,1);
      lcd.print(F("DO IT SOFTLY..."));
      COIN(1); // then countdown play some music here 
      GetButtonStatus();
      
      // Check Buttons Status
      int a = 0;
      while (a == 0){
        GetButtonStatus();
        if (ButtonStatus[2]) { a = LED_YELLOW; }     
        if (ButtonStatus[3]) { a = LED_GREEN; }       
        if (ButtonStatus[4]) { a = LED_RED; }         
        if (ButtonStatus[5]) { a = LED_BLUE; }  
        if (ButtonStatus[1]) { CloseGame(); return; }  // BLACK Button Rules
        ClearButtonStatus();
      }
    
      // Level Result
      lcd.clear();
      lcd.print(F("Winner is "));
      switch (a) {
        case LED_YELLOW: lcd.print(F("YELLOW")); break;
        case LED_GREEN:  lcd.print(F("GREEN"));  break;
        case LED_RED:    lcd.print(F("RED"));    break;
        case LED_BLUE:   lcd.print(F("BLUE"));   break;
      }
      PlayColor(a);
      GameRate[GetColorPin(a)]++;
      PowerOnButtonLED();
      lcd.setCursor(0,1);
      lcd.print(F("<PLAY      EXIT>"));
    
      // wait until a button is pressed
      //while (!ButtonStatus[0] && !ButtonStatus[1]){ GetButtonStatus(); }
      delay(random(500, 1000)); // random delay 

      // BLACK Button Press
      if (ButtonStatus[1]) {
        CloseGame(); // set back to initial state
        return;
        }
    ONEUP();
    //delay(1000);
    } //end for WHILE game
    
    // Show Absolute Winner
    PowerOffButtonLED();
    int maxscore = 0;
    for (int i = 0; i < 4; i++) { if (GameRate[i] > maxscore) { maxscore = GameRate[i]; } }
    for (int i = 0; i < 4; i++) { if (GameRate[i] == maxscore) { status_leds[i] = true; } }
    lcd.clear();
    lcd.print(F("ABSOLUTE  WINNER"));
    lcd.setCursor(0,1);
    lcd.print(F("<PLAY      EXIT>"));
    FLAGPOLEFANFARE();
    ClearLine();
    lcd.print(F("  WINNER IS...  "));
    PowerOnButtonLED();
    delay(3000);
    CloseGame();
}
