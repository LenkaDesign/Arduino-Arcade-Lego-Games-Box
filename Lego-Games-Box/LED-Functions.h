//Init LED Array
bool status_leds[4] = {false, false, false, false};

// Power On Button LEDs
void PowerOnButtonLED () {
  for(int i=0; i<4; i++) {
    digitalWrite(pin_leds[i], (status_leds[i]) ? HIGH : LOW);
  }
}


// Set Active Button LED
void SetButtonLED (byte LED_color, bool power = true) {
  int i = -1;
  i = GetColorPin(LED_color);
  if (i >= 0) {status_leds[i] = !status_leds[i];}
  if (power) { PowerOnButtonLED(); }
}


// Power Off Button LEDs
void PowerOffButtonLED () {
  for(int i=0; i<4; i++) { status_leds[i] = false; }
  PowerOnButtonLED();
}
