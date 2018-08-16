// Array of Players Name
String players[4] = {"CHINA","NORTH KOREA","RUSSIA","USA"};
// BLUE RED YELLOW GREEN
// LED_YELLOW, LED_GREEN, LED_RED, LED_BLUE
bool InGame[4] = {false,false,false,false};

//Check InGame Array
byte HowManyInGame() {
  byte num = 0;
  for (int i=0; i<4; i++) {
    if (InGame[i]) {num++;}
    }
  return num;
}

// MAIN GAME Function
//----------------------------------
void WarGame() {
  GameType = 5;
  bool GameOver = false;
  int a = 0;
  byte InNum = 0;
  byte MyTarget[4] = {0,0,0,0};
  int interval = 1000;
  bool act = false;
  byte CurrPlayer = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;

  for (int i=0; i<4; i++) { InGame[i] = false;}

  //Who Play?
  lcd.clear();
  lcd.print(F("WHO WANNA PLAY?"));
  lcd.setCursor(0,1);
  ClearButtonStatus();
  PowerOffButtonLED();
  while ((!ButtonStatus[0])||(InNum<2))  {
    ClearButtonStatus();
    GetButtonStatus();

    if (ButtonStatus[2]) { a = LED_YELLOW;} // YELLOW
    if (ButtonStatus[3]) { a = LED_GREEN; } // GREEN         
    if (ButtonStatus[4]) { a = LED_RED; }   // RED         
    if (ButtonStatus[5]) { a = LED_BLUE; }  // BLUE
    if (ButtonStatus[1]) { CloseGame(); return; }  // BLACK Button Rules
      
    // BLACK Button Rules  
    if (ButtonStatus[1]) { CloseGame(); return; }

    // New Player Push
    if (a != 0 ) {
        ClearLine(1);
        lcd.setCursor(0,1);

        SetButtonLED(a);

        InGame[GetColorPin(a)] = !InGame[GetColorPin(a)];
        if (InGame[GetColorPin(a)]) { lcd.print(players[GetColorPin(a)]);  beep(PIN_PLAYER_1,ButtonNote[GetColorPin(a)],100); beep(PIN_PLAYER_2,MI5,70);}
            else { lcd.print(players[GetColorPin(a)]); lcd.print(F(" out!")); beep(PIN_PLAYER_1,MI5,70); beep(PIN_PLAYER_2,ButtonNote[GetColorPin(a)],100);}

        InNum = HowManyInGame();
        a = 0;

        PowerOnButtonLED();
        
        delay(500);
        ClearLine(1);
        lcd.setCursor(0,1);
      } else {
        lcd.setCursor(0,1);
        if (InNum > 0 ) {
        lcd.print(F("GAME FOR "));
        lcd.print(InNum);
        } else { lcd.print(F("  *NO PLAYERS*  ")); }
      }
  } // End While WHOPLAY

    while (!GameOver) {
    // Set Target
    // Loop for all Players
    for (int i = 0; i < 4; i++)
      {
        ClearButtonStatus();
        // Do for Player Active
        if (InGame[i]) {
          PowerOffButtonLED(); // PowerOff All LED
          // On LED for CurrentPlayer
          SetButtonLED(pin_leds[i]);
          // Display info
          lcd.clear();
          lcd.print(players[i]);
          lcd.setCursor(0,1);
          lcd.print(F("CHOOSE TARGET"));

          // Player choose other Active Player
          ClearButtonStatus(); // Status all button FALSE
          // WHITE button = Confirm Select target 
          MyTarget[i] = 0; 
          while ((!ButtonStatus[0])||(MyTarget[i] == 0)) {
            // Get Button Status
            GetButtonStatus();
            a = 0; // CurrButton

            // Save button state IF Button - Press,  Player in Game, Current Button (i) not Target Color
            if ((ButtonStatus[2])&&(InGame[GetColorPin(LED_YELLOW)])&&(GetColorPin(LED_YELLOW) != i)) { a = LED_YELLOW;}      // YELLOW
              else if ((ButtonStatus[3])&&(InGame[GetColorPin(LED_GREEN)])&&(GetColorPin(LED_GREEN) != i)) { a = LED_GREEN; } // GREEN         
              else if ((ButtonStatus[4])&&(InGame[GetColorPin(LED_RED)])&&(GetColorPin(LED_RED) != i)) { a = LED_RED; }       // RED         
              else if ((ButtonStatus[5])&&(InGame[GetColorPin(LED_BLUE)])&&(GetColorPin(LED_BLUE) != i)) { a = LED_BLUE; }    // BLUE
            
            if (ButtonStatus[1]) { CloseGame(); return; }  // BLACK Button Rules
            
            // If CurrButton change and ButtonPress - Save New Target
            if (a != 0)
              {
                beep(PIN_PLAYER_1,LA5,100); beep(PIN_PLAYER_2,MI5,70);
                MyTarget[i] = a; // Get ID from pin_leds[];
                //PlayColor(a); // Play Target Music
                //if (!status_leds[i]) {SetButtonLED(pin_leds[i]);} // Set LED for Player if OFF
                for (int j = 0; j < 4; j++) 
                  {
                    if (j == i)  {status_leds[j] = true;}
                      else if (j == GetColorPin(a)) {status_leds[j] = !status_leds[j];}
                      else {status_leds[j] = false;}
                  }
                //SetButtonLED(a); // Set LED for Target or Off if prevs
                ClearLine(1); // Clear Second line LCD
                lcd.setCursor(0,1);
                if (!status_leds[GetColorPin(a)]) 
                {
                  MyTarget[i] = 0;
                  lcd.print(F("CHOOSE TARGET"));
                } else { lcd.print((char)B01111110); lcd.print(F(" ")); lcd.print(players[GetColorPin(a)]); }
              PowerOnButtonLED(); // ON All Active LED
              delay(300);
              ClearButtonStatus();
              }
            } // End for While Target Select for One Player
          } // Only for Active Player
        } // For across All Players
     
      // Easy Game
      PowerOffButtonLED();
      ClearButtonStatus();
      
      // Init state
      lcd.clear();
      PowerOffButtonLED();
      lcd.print(F("Who is First?"));
      lcd.setCursor(0,1);
      lcd.print(F("Get Ready..."));
      delay(random(1000, 2000)); // random delay 
      lcd.clear();
      lcd.print(F("GIVE ME MONEY$$$"));
      lcd.setCursor(0,1);
      lcd.print(F("ROCKET JUMP..."));
      COIN(1); // then countdown play some music here 
      GetButtonStatus();
      
      // Check Buttons Status
      a = 0;
      act = false;
      while ((a == 0)||(!act)) {
        GetButtonStatus();
        if (ButtonStatus[2]) { a = LED_YELLOW; }     
        if (ButtonStatus[3]) { a = LED_GREEN; }       
        if (ButtonStatus[4]) { a = LED_RED; }         
        if (ButtonStatus[5]) { a = LED_BLUE; }  
        if (ButtonStatus[1]) { CloseGame(); return; }  // BLACK Button Rules
        if ( a != 0) { act = InGame[GetColorPin(a)]; }
        ClearButtonStatus();
      }
      
      // Rocket Jump
      lcd.clear();
      lcd.print(F("ROCKET GO TO..."));
      lcd.setCursor(0,1);
      CurrPlayer = MyTarget[GetColorPin(a)];
      SetButtonLED(CurrPlayer);
      lcd.print(players[GetColorPin(CurrPlayer)]);
      
      // Waiting User Action
      act = false;
      lcd.setCursor(0,0);
      ClearButtonStatus();
      currentMillis = previousMillis = millis();
      while (currentMillis - previousMillis <= interval)
      {
        // Fill Line
        if ((currentMillis - previousMillis) % (interval/16) == 0) lcd.print((char)255);
        if (!act) { // Get Button Status
          GetButtonStatus();
          if (ButtonStatus[GetColorPin(CurrPlayer)+2]) { act = true;}     
          }
        currentMillis = millis();
      }
      // Clear Status
      ClearButtonStatus();
      
      lcd.clear();
      DAMAGEWARP();
      lcd.print(players[GetColorPin(CurrPlayer)]);
      if (act) {
          lcd.setCursor(0,1);
          lcd.print(F("STAY ALIVE! =)"));
          RESCUEFANFARE();
        } else {
          lcd.setCursor(0,1);
          lcd.print(F("WAS DESTROYED :("));
          DEATH();
          InGame[GetColorPin(CurrPlayer)] = false;
        }

      PowerOffButtonLED();

      for (int i = 0; i < 4; i++) {
        if (InGame[i]) {SetButtonLED(pin_leds[i]);}
      }
        
      InNum = HowManyInGame();
      if (InNum >1 )
      {
        PowerOnButtonLED();
        // Next Level Interval
        if (interval > 150) { interval = interval - 50;}
        lcd.clear();
        lcd.print(F("..:NEXT LEVEL:.."));
        lcd.setCursor(0,1);
        lcd.print(F("TIME: "));
        lcd.print(interval);
        delay(1000);        
      } else {
        GameOver = true;
        for (int i = 0; i < 4; i++) {
          if (InGame[i]) {CurrPlayer = i;}
          }
      }
    }
      
  // Show Winner
  lcd.clear();
  lcd.print(F(" YOU ARE ALONE! "));
  lcd.setCursor(0,1);
  lcd.print(players[CurrPlayer]);
  FLAGPOLEFANFARE();
  PowerOnButtonLED();
  lcd.print(F(" WINS"));
  delay(3000);
  CloseGame(); 
  return;
}
