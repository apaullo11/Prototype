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

// Screen Dimension Definitions
#define LCDCOLUMNS        16
#define LCDROWS           2
#define OLEDWIDTH         128
#define OLEDHEIGHT        64

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
  // Mechanical
  #define JOYSTICKXPIN    A0
  #define JOYSTICKYPIN    A1
  #define JOYSTICKCLK     8
//---- END OF PIN DEFINITIONS ----//

//---- DATA STRUCTURING ----//
  // ENUM DEFS
  enum LCDAlignment       { left, center, right };
  enum EmuState           { selection, snake, pong, doom };
  enum GameStates         { unactivated, activated, playing, failure = -1 };

  struct LCDText {
    LCDText(String t, LCDAlignment a);
    // String to be printed to the LCD display
    String text = "";
    // Preferred alignment of the text on the LCD display (left by default)
    LCDAlignment align = left; 
  };
//---- END OF DATA STRUCTURES ----//

Adafruit_SSD1306 OLED(OLEDWIDTH, OLEDHEIGHT);
LiquidCrystal lcd = LiquidCrystal(LCDRS, LCDENABLE, D4, D5, D6, D7);

EmuState emu = selection;
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
    break;

    case (doom):
    break;

    default:
    break;
  }

}

/*
template <typename T>
int arrSize(T t[]) {
  return t/sizeOf(T);
}
*/

// LCDText object constructor with String and LCDAlignment inputs to set initial vars
LCDText::LCDText(String t, LCDAlignment a) {
  text = t;
  align = a;
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