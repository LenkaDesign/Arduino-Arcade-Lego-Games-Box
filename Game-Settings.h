#define GAME_VER "v.4"
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define BUTTON_PIN A1 // Multiple Button Pin - it is a internal pullup pin hence A1.

#define RANDOM_PIN A0

// Game change Buttons
#define WHITE_BUTTON 2
#define BLACK_BUTTON 3

// LED pins (inside big buttons)
#define LED_BLUE 4 
#define LED_RED 5 
#define LED_YELLOW 6 
#define LED_GREEN 7 

bool ButtonStatus[7] = {false, false, false, false, false, false, false};

// Define speaker pins (PWM~) [REQUIRED]
#define PIN_PLAYER_1 11
#define PIN_PLAYER_2 10

// Define ground pins for display [OPTIONAL]
//#define DISPLAY_1 13
//#define DISPLAY_2 12

Tone player_1;
Tone player_2;

byte GameState = 0;
byte GameType = 1;
int  GameLevel = 0; 
bool GameSound = true;
unsigned int GameRand = 777;
unsigned long ActiveTime; // Global Game Timer

word InMemory[2] = {0,0}; // For Store in EEPROM
// 0 - Memory, 1 - Reaction
int eeAddress = 0;

String GameTitle[] = {"Memory Game", "Reaction Game", "Tournament Game", "Melody Game", "Nuclear War Game"};
// number of items in an array
#define NUMGAME(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

//int tempo; remove if compiles ok.

// This value is higher and the notes are separated from each other. [OPTIONAL]
int tronca = 15;

// You can set speed limits [OPTIONAL]
int tempo_minimo = 78;
int tempo_massimo = 500;

// Define the order of the pins in the display so that they are ordered according to the universal convention [OPTIONAL]
byte pin_leds[4] = {LED_YELLOW, LED_GREEN, LED_RED, LED_BLUE};

// Return Element Number for Color from "pin_leds"
int GetColorPin (byte LED_color) {
  for(int i=0; i<4; i++){
    if (pin_leds[i] == LED_color) {return i;}
  }
  return -1;
}

// Debounce Variables
unsigned long lastDebounceTime = 0;
int lastButtonState = -1;

// Return Number for Button
int ValueToButton(int c) {
  if (c>430 && c<500) { return 5; }  // BLUE   5
  else if (c>330 && c<410) { return 4; }  // RED    4
  else if (c>190 && c<270) { return 2; }  // YELLOW 2
  else if (c>5 && c<20)    { return 3; }  // GREEN  3
  return -1;
}

// Get Button Status
void GetButtonStatus () {
  int c = 0;
  bool AnyKey = false;
  int DebounceC = 0;
  int DebounceTime = 30;

  c = analogRead(BUTTON_PIN); // get the analog value 
   
  DebounceC = analogRead(BUTTON_PIN); // get new analog value  
  if (DebounceC > c) { c = DebounceC; }
  
  if ((ValueToButton(c) != lastButtonState)&&(ValueToButton(c) > 0)) {
    lastDebounceTime = millis();
  }

  if ((ValueToButton(c) > 0)&&((millis() - lastDebounceTime) > DebounceTime))
    {
     for (int i = 2; i <= 5; i++) {
      if (ValueToButton(c) == i) {ButtonStatus[i] = true; AnyKey = true;}
        else {ButtonStatus[i] = false;}
      }
    }

  lastButtonState = ValueToButton(c);
  
  if  (digitalRead(WHITE_BUTTON) == LOW) { ButtonStatus[0] = true; AnyKey = true;}
   // else  { ButtonStatus[0] = false; }
  if  (digitalRead(BLACK_BUTTON) == LOW) { ButtonStatus[1] = true; AnyKey = true;}
  //  else  { ButtonStatus[1] = false; }
  
  ButtonStatus[6] = AnyKey; // Button Pressed
  if (ButtonStatus[6]) { ActiveTime = millis(); } // Counter for Global Game Timer
  if (millis() - ActiveTime > 300000) { ButtonStatus[1] = true; ButtonStatus[6] = true; GameSound = true;} // Timer for Close Any Game
}

// Clear Status for ALL Button
void ClearButtonStatus () {
  for (int i = 0; i < 7; i++) { ButtonStatus[i] = false; }
}

// Set Default Values
void CloseGame() {
  GameState = 0; 
  GameLevel = 0;
  ClearButtonStatus();
  lcd.clear();
  lcd.print(GameTitle[GameType-1]);
  lcd.setCursor(0,1);
  lcd.print(F("BLACK to CHANGE"));
}

// Clear line on LCD
void ClearLine(int line = 0) {
  lcd.setCursor(0,line);
  lcd.print(F("                "));
  lcd.setCursor(0,line);
}

