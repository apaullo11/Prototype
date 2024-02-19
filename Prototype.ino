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
  #define ROTARYENCCLK    11
//---- END OF PIN DEFINITIONS ----//

//---- DATA STRUCTURING ----//
  // ENUM DEFS
  enum LCDAlignment       { left, center, right };
  enum EmulatorState      { selection, snake, pong, tron, doom };
  enum GameStates         { unactivated, activated, playing, failure = -1 };
  enum RotaryEncoder      { ccw = -1, none, cw, clk };

  struct LCDText {
    LCDText(String t, LCDAlignment a);
    // String to be printed to the LCD display
    String text;
    // Preferred alignment of the text on the LCD display (left by default)
    LCDAlignment align; 
    // Row String is to be printed to
    // int row = 0
  };

  int rotEncPos = 0;
  int8_t rotEncDir = 0;
  uint8_t rotEncState;
  bool rotEncClk;
//---- END OF DATA STRUCTURES ----//

Adafruit_SSD1306 OLED(OLEDWIDTH, OLEDHEIGHT);
LiquidCrystal lcd = LiquidCrystal(LCDRS, LCDENABLE, D4, D5, D6, D7);

EmulatorState emu = selection;
EmulatorState gameSelect;
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

  rotEncState = GetRotaryState(ROTARYENCPINA, ROTARYENCPINB);
  rotEncClk = digitalRead(ROTARYENCCLK);
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
  
  OLED.setCursor(padding[0], padding[1]);

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
  
  // TODO - Draw PLAY & QUIT OPTIONS

}

void LCDRunGame(EmulatorState game) {
  LCDPrint(lcd, LCDText("Running:",center));

  DrawGameMenu(game);
}

// Polls the Rotary Encoder for changes in position or presses
// - Rotating takes precedent over clicking
RotaryEncoder PollRotaryEnc() {
  rotEncDir += GetRotaryKnobDir(&rotEncState, ROTARYENCPINA, ROTARYENCPINB);
  // if at rest pos and the direction total is greater than a 4 state dif in one direction
  if ( (rotEncState == 3) && ( abs(rotEncDir) >= 4) ) {
    return RotaryEncoder(rotEncDir>>2);
  }

  if ( (rotEncClk != digitalRead(ROTARYENCCLK)) && (rotEncClk == HIGH) ) {
    rotEncClk = digitalRead(ROTARYENCCLK);
    return clk;
  }
}

// Checks the Rotary Encoder Pins and returns the state number
// - Pin A has a value of 1; Pin B is bit shifted to a value of 2; OR bit operator to combine into one var
// - 3 is both HIGH; 1 is only A HIGH; 0 is both LOW; 2 is B HIGH || 4 states in 2 bits
uint8_t GetRotaryState(const uint8_t PinA, const uint8_t PinB) {
  return digitalRead(PinA) | ( digitalRead(PinB)<<1 );
}

// Returns the position a Rotary Encoder has been turned to based off an input original state and the Pin numbers
// - Returns 1,-1, or 0 || 1 is CW, -1 is CCW, and 0 is no movement
int8_t GetRotaryKnobDir(uint8_t *oldState, const uint8_t PinA, const uint8_t PinB) {
  uint8_t curState;
  int8_t knobDir;
  
  curState = GetRotaryState(PinA, PinB);
  
  const int8_t KNOBDIR[] = {
    0, -1,  1,  0,
    1,  0,  0, -1,
   -1,  0,  0,  1,
    0,  1, -1,  0
  };

  // Takes advantage of the 2-bit encoder state representation by combining the two vars with one bitshifted
  // This creates 16 unique states that can be resolved into different directions 
  knobDir = KNOBDIR[ curState | ( (*oldState)<<2 ) ];
  
  // changes input state variable to match current state
  (*oldState) = curState;
  return knobDir;
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