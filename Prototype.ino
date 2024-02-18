//---- LIBRARIES AND INCLUDE FILES ----//
  #pragma once
  // Libraries
  #include <Adafruit_SSD1306.h>
  #include <splash.h>
  #include <LiquidCrystal.h>

  // BITMAPS
  //#include "Bitmaps/SnakeMenuPlay.h"
  //#include "Bitmaps/SnakeMenuQuit.h"
//---- END OF LIBRARIES AND INCLUDE FILES ----//

// Screen Definitions
#define LCDCOLUMNS        16
#define LCDROWS           2
#define OLEDWIDTH         128
#define OLEDHEIGHT        64
#define OLEDLETTERW       6
#define OLEDLETTERH       8

//---- PIN DEFINITIONS ----//
  // LCD Display
  #define LCDRS           7
  #define LCDENABLE       6
  #define D4              5
  #define D5              4
  #define D6              3
  #define D7              2
  // OLED Display (Definitions not used - just for reference)
  #define SDA             A4
  #define SCL             A5
  // Interactive
  #define JOYSTICKXPIN    A0
  #define JOYSTICKYPIN    A1
  #define JOYSTICKCLK     8
  #define ROTARYENCPINA   9
  #define ROTARYENCPINB   10
//---- END OF PIN DEFINITIONS ----//

//---- DATA STRUCTURING ----//
  // ENUM DEFS
  enum LCDAlignment       { left, center, right };
  enum EmulatorState      { selection, snake, pong, tron, doom };
  enum GameStates         { unactivated, activated, playing, failure = -1 };

  struct LCDText {
    LCDText(String t, LCDAlignment a);
    // String to be printed to the LCD display
    String text;
    // Preferred alignment of the text on the LCD display (left by default)
    LCDAlignment align; 
    // Row String is to be printed to
    // int row = 0
  };
//---- END OF DATA STRUCTURES ----//

Adafruit_SSD1306 OLED(OLEDWIDTH, OLEDHEIGHT);
LiquidCrystal lcd = LiquidCrystal(LCDRS, LCDENABLE, D4, D5, D6, D7);

EmulatorState emu = selection;
EmulatorState select;
GameStates game1 = unactivated;
GameStates game2 = unactivated;
GameStates game3 = unactivated;

void setup() {
  // lcd init
  lcd.begin(LCDCOLUMNS, LCDROWS);
  lcd.noCursor();
  lcd.clear();

  LCDPrint(lcd, LCDText("Left",left), LCDText("Right",right));

  // prevent main loop from starting if state vars aren't initialized properly
  while (emu != selection || game1 != unactivated || game2 != unactivated || game3 != unactivated);
}

void loop() {
  // main game states
  switch (emu) {
    // game selection state
    case (selection):

    break;

    case (snake):
      switch (game1) {
        // boot game
        case (unactivated):
          game1 = activated;
          LCDRunGame(snake);
        break;

        // Menu Selection
        case (activated):
        
        break;

        // game loop
        case (playing):
        break;

        // game over
        case (failure):
        break;

        default:
        break;
      }
    break;

    case (pong):
      switch (game2) {
        // boot game
        case (unactivated):
          
          game2 = activated;
          LCDRunGame(pong);
        break;

        // Menu Selection
        case (activated):
        
        break;

        // game loop
        case (playing):
        break;

        // game over
        case (failure):
        break;

        default:
        break;
      }
    break;

    case (tron):
      switch (game3) {
        // boot game
        case (unactivated):
          
          game3 = activated;
          LCDRunGame(tron);
        break;

        // Menu Selection
        case (activated):
        
        break;

        // game loop
        case (playing):
        break;

        // game over
        case (failure):
        break;

        default:
        break;
      }
    break;

    default:
    break;
  }

}

// LCDText object constructor with String and LCDAlignment inputs to set initial vars
LCDText::LCDText(String t, LCDAlignment a) { //, int r) {
  text = t;
  align = a;
  // row = r
}

void DrawGameMenu(EmulatorState game) {
  OLED.clearDisplay();
  OLED.drawRect(0, 0, OLEDWIDTH, OLEDHEIGHT, 1);
  OLED.setTextColor(1);
  OLED.setTextSize(3);

  uint8_t letters = 0;
  switch (game) {
    case (snake):
      letters = 5;
    break;
    case (pong):
      letters = 4;
    break;
    case (doom):
      letters = 4;
    break;
  }

  uint8_t padding[2] = {0,0}; 
  // center game title text horizontally
  padding[0] = (OLEDWIDTH - (letters * 3 * OLEDLETTERW)) / 2;
  // have bottom line of text on middle horizontal
  padding[1] = (OLEDHEIGHT / 2) - (letters * 3 * OLEDLETTERH);
  
  switch (game) {
    case (snake):
      OLED.write("Snake");
    break;
    case (pong):
      OLED.write("Pong");
    break;
    case (doom):
      OLED.write("Doom");
    break;
  }

  OLED.setTextSize(2);
  

}

void LCDRunGame(EmulatorState game) {
  LCDPrint(lcd, LCDText("Running:",center));

  DrawGameMenu(game);
}

// Polls the Rotary Encoder Pins and returns the state number
// - Pin A has a value of 1; Pin B is bit shifted to a value of 2; OR operator to combine
// - 3 is both HIGH; 1 is only A HIGH; 0 is both LOW; 2 is B HIGH
uint8_t GetRotaryState() {
  return digitalRead(ROTARYENCPINA) | (digitalRead(ROTARYENCPINB)<<1);
}

int8_t PollRotaryEnc() {

  while (false);
}

// Prints Strings to an input LCD display and adjusts the text positioning based on an input LCDText data structure
// - Text that is longer than the column number will be cut off (limited to a max of 128 columns)
// - Any args provided that surpass the row number will not be used (limited to a max of 128 rows)
void LCDPrint(LiquidCrystal LCD, LCDText t, ...) {
  // prepare list of all args
  va_list list;
  va_start(list, t);

  for (uint8_t i = 0; i < LCDROWS; i++) {

    // set text indent based on input alignment - if length is greater than columns, set to 0
    uint8_t indent = (LCDCOLUMNS < t.text.length()) ? 0 : (LCDCOLUMNS - t.text.length()) ; 
    switch (t.align) {
      // no indent
      case (left):
        indent = 0;
        break;

      // half of column difference to center text
      case (center):
        indent = indent/2;
        break;

      // keep indent as column difference
      case (right):
        break;

      default:
        indent = 0;
        break;
    }
    LCD.setCursor(indent,i);
    LCD.print(t.text.c_str());

    // to next LCDText object
    va_arg(list, LCDText);
  }

  va_end(list);
}