//Current Notes
int CurrNotes[4] = {0,2,4,6};
byte Octave = 1;

// Change CurrNotes Array
void ChangeStan (int x) {
  for (int i = 0; i < 4; i++ ) { 
    // Only for others Buttons
    if (i != x) {
      do 
      {
        if (CurrNotes[i] % 2 != 0) {CurrNotes[i] = CurrNotes[i] - 1;}
        CurrNotes[i] = CurrNotes[i] + 2; 
        if (CurrNotes[i] > 11) {CurrNotes[i] = CurrNotes[i] - 12;}
        } while((CurrNotes[i] == CurrNotes[x])||(CurrNotes[i] == CurrNotes[x]-1));
    } else {
      if (CurrNotes[x] % 2 == 0) {CurrNotes[x] = CurrNotes[x] + 1;}
        else  {CurrNotes[x] = CurrNotes[x] - 1;}
    }
  }
}

// Loop Change Octaves
void ChangeOctave() {
  Octave++;
  if (Octave >= 3) {Octave = 0;}
}

// Main Game Function
void MelodyGame () {
  int PrevNote = -1;
  float DurNote = 3;
  bool NotesShow = false;
  GameType = 4;
  //Welcome
  lcd.clear();
  lcd.print(F("  MELODY GAME   "));
  lcd.setCursor(0,1);
  lcd.print(F(" BLACK to EXIT! "));
  PowerOffButtonLED();
  ClearButtonStatus();
  delay(1000);
  while(!ButtonStatus[1]) // BLACK button press
    { 
      ClearButtonStatus();
      GetButtonStatus();
      if (ButtonStatus[6])
        {
          // Loop for all color BUTTONs
          for (int i = 2; i < 6; i++)
            {
              // Play Buttons
              if (ButtonStatus[i]) { 
                SetButtonLED (pin_leds[i-2]);
                suonaCoppia(NoteStan[Octave][CurrNotes[i-2]], DurNote, false); 
                ButtonStatus[i] = false;
                if (NoteStan[Octave][CurrNotes[i-2]] != PrevNote) { 
                  ClearLine(1);
                  lcd.setCursor(0,1);
                  lcd.print(F("NOTE PLAYS: "));
                  lcd.print(StrNoteStan[CurrNotes[i-2]]);
                  PowerOffButtonLED();
                  DurNote = 3;
                  PrevNote = NoteStan[Octave][CurrNotes[i-2]];
                  } else if (abs(NoteStan[Octave][CurrNotes[i-2]]-PrevNote) == 1) {DurNote  = DurNote + 1;}
                ChangeStan (i-2);
                NotesShow = false;
                }
            } // end loop
          if (ButtonStatus[0]) 
            {
              ChangeOctave();
              ClearLine(1);
              lcd.setCursor(0,1);
              lcd.print(F("OCTAVE #"));
              lcd.print(Octave+3);
              delay(300);
            } else {
              SetButtonLED (pin_leds[random(0,4)]);
              }
        } else {
          // Show Current Notes Array
          if (!NotesShow)
          {
            ClearLine(1);
            lcd.setCursor(0,1);
            for (int i = 0; i < 4; i++) { lcd.print(StrNoteStan[CurrNotes[i]]); lcd.print(' '); NotesShow = true;}
          }
        }
      if (millis() - ActiveTime > 2500) {SetButtonLED (pin_leds[random(0,4)]); ActiveTime = millis();}
    }

  lcd.print(F("   THANK YOU!   "));
  lcd.setCursor(0,1);
  lcd.print(F("   GOOD BYE;)   "));
  delay(3000);
  CloseGame(); 
  return;
}

