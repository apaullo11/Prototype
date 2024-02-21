//---- LIBRARIES AND INCLUDE FILES ----//
  #pragma once
  // Libraries
  #include <Adafruit_SSD1306.h>
  #include <splash.h>
  #include <LiquidCrystal.h>

  #include "LinkedList.h"

  // BITMAPS
  //#include "Bitmaps/SnakeMenuPlay.h"
  //#include "Bitmaps/SnakeMenuQuit.h"
//---- END OF LIBRARIES AND INCLUDE FILES ----//

// Screen Definitions
#define LCDCOLUMNS            16
#define LCDROWS               2
#define OLEDWIDTH             128
#define OLEDHEIGHT            64
#define OLEDLETTERW           6
#define OLEDLETTERH           8

//---- PIN DEFINITIONS ----//
  // LCD Display
  #define LCDRS               7
  #define LCDENABLE           6
  #define D4                  5
  #define D5                  4
  #define D6                  3
  #define D7                  2
  // OLED Display (Definitions not used - just for reference)
  #define SDA                 A4
  #define SCL                 A5
  // Interactive
  #define JOYSTICKXPIN        A0
  #define JOYSTICKYPIN        A1
  #define JOYSTICKCLK         8
  #define ROTARYENCPINA       9
  #define ROTARYENCPINB       10
  #define ROTARYENCCLK        11
  #define BUTTONPIN           12
//---- END OF PIN DEFINITIONS ----//

//---- DATA STRUCTURING ----//
  // ENUM DEFS
  enum LCDAlignment           { left, center, right };
  enum EmulatorState          { selection, snake, pong, tron, doom };
  enum GameStates             { unactivated, activated, playing, failure = -1 };
  enum RotaryEncoder          { ccw = -1, none, cw, clk };

  struct LCDText {
    LCDText(String t, LCDAlignment a);
    // String to be printed to the LCD display
    String text;
    // Preferred alignment of the text on the LCD display (left by default)
    LCDAlignment align; 
    // Row String is to be printed to
    // int row = 0
  };


  int rotEncOldPos = 0;
  int rotEncCurPos = 0;
  uint8_t rotEncState;
  bool rotEncClk;

  unsigned long t0 = millis();
  unsigned long t1;
  unsigned long lastUpdate;
  float gameSpeed = 1.0;
//---- END OF DATA STRUCTURES ----//

Adafruit_SSD1306 OLED(OLEDWIDTH, OLEDHEIGHT);
LiquidCrystal lcd = LiquidCrystal(LCDRS, LCDENABLE, D4, D5, D6, D7);

enum EmulatorState emu = selection;
enum EmulatorState gameSelect = snake;
enum GameStates game1 = unactivated;
enum GameStates game2 = unactivated;
enum GameStates game3 = unactivated;
LinkedList *SnakeGame;
vec2 snakeFruit;
unsigned int score;

void setup() {
  // random seed of unconnected analog input as recommended by Arduino documentation
  randomSeed(analogRead(A2));

  // lcd init
  lcd.begin(LCDCOLUMNS, LCDROWS);
  lcd.noCursor();
  lcd.clear();

  //LCDPrint(lcd, LCDText("Left",left), LCDText("Right",right));

  // PIN MODES
  pinMode(LCDRS, OUTPUT);
  pinMode(LCDENABLE, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(ROTARYENCPINA, OUTPUT);
  pinMode(ROTARYENCPINB, OUTPUT);
  pinMode(ROTARYENCCLK, OUTPUT);
  pinMode(BUTTONPIN, OUTPUT);


  // prevent main loop from starting if state vars aren't initialized properly
  while (emu != selection || game1 != unactivated || game2 != unactivated || game3 != unactivated);

  rotEncState = GetRotaryState(ROTARYENCPINA, ROTARYENCPINB);
  rotEncClk = digitalRead(ROTARYENCCLK);
  t1 = millis();
  lastUpdate = 0;
}

void loop() {
  // main game states
  switch (emu) {
    // game selection state
    case (selection):
      enum RotaryEncoder rotEncInput = PollRotaryEnc();
      switch (rotEncInput) {
        // Game is selected
        case (clk):
          
        break;

        case (none):
        break;

        // Rotary Encoder turned cw or ccw
        default:
          // EmulatorStates game states start at 1 and not 0 -> decrement 1 before modulus and increment 1 after 
          gameSelect = EmulatorState( ((gameSelect -1 + rotEncInput) % 3) + 1 );
          LCDSelectGame(gameSelect);
        break;
      }
    break;

    case (snake):
      switch (game1) {
        // boot game
        case (unactivated):
          game1 = activated;
          BootGame(snake);
        break;

        // Menu Selection
        case (activated):

          // if play, run game initalizer
          if (true) StartSnake();
          else if (false) QuitGame();
        break;

        // game loop
        case (playing):
          vec2 dir;
          // get direction of snake
          // check upcoming state, set game1 to failure if game over, increment score if needed (and pick next fruit);
          // return
          if (game1 != failure) {
            SnakeNextFrame(dir);
          }
        break;

        // game over
        case (failure):
          // remove linked list
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
          BootGame(pong);
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
          BootGame(tron);
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
  t0 = t1;
  t1 = millis();
}

// ---- CONSTRUCTORS ---- //
  LCDText::LCDText(String text, LCDAlignment align) { //, int row) {
    this->text = text;
    this->align = align;
    // this->row = row
  }

// ---- END OF CONSTRUCTORS ---- //

void LCDSelectGame(EmulatorState game) {
  LCDPrint(lcd, LCDText("Selected Game:", center), LCDText(EmuStateToString(game), center));
  
}

void BootGame(EmulatorState game) {
  LCDPrint(lcd, LCDText("Running:", center), LCDText(EmuStateToString(game), center));

  // TODO - Draw logo? lol

  DrawGameMenu(game);

}

void DrawGameMenu(EmulatorState game) {
  OLED.clearDisplay();
  OLED.drawRect(0, 0, OLEDWIDTH, OLEDHEIGHT, 1);
  OLED.setTextColor(1);
  OLED.setTextSize(3);

  uint8_t padding[2] = {0,0}; 
  // center game title text horizontally
  padding[0] = (OLEDWIDTH - (EmuStateToString(game).length() * 3 * OLEDLETTERW)) / 2;
  // have bottom line of text on middle horizontal
  padding[1] = (OLEDHEIGHT / 2) - (EmuStateToString(game).length() * 3 * OLEDLETTERH);
  
  OLED.setCursor(padding[0], padding[1]);
  OLED.print(EmuStateToString(game));

  OLED.setTextSize(2);
  
  // TODO - Draw PLAY & QUIT OPTIONS
  // FEATURE - Add Scoreboard?

}

void GameMenuSelect() {
  vec2 axisVals = GetJoystickAxes(JOYSTICKXPIN,JOYSTICKYPIN);

}

void QuitGame() {
  // TODO
}

void NewSnakeNode(LinkedList *s, vec2 *pos, vec2 *dir) {
  s->addTailNode();
  s->tail->pos = pos;
  s->tail->dir = dir;
}

void StartSnake() {
  lastUpdate = 0;
  for (uint8_t i = 0; i<4; i++) {
    NewSnakeNode(SnakeGame, new vec2(OLEDWIDTH/2,(OLEDHEIGHT/2)+i), new vec2(0,-1));
  }

}

bool isSnakeDirValid(vec2 dir) {
  vec2 nextPos = add(*(SnakeGame->head->pos), dir);
  if ( !OLEDGetPixel(OLED, nextPos) ) {
    return true;
  } else if ( (nextPos.x == snakeFruit.x) && (nextPos.y == snakeFruit.y) ) {
    score++;
    return true;
  } else return false;

}

// Pass pixel info to DrawNextFrame
void SnakeNextFrame(vec2 dir) {
  // calculate when next frame
  // is it time for next frame?
  if (false) return;

  //DrawNextFrame(OLED);

  lastUpdate = 0;
}

// Polls the Rotary Encoder for changes in position or presses
// - Rotating takes precedent over clicking (NVM CLICKING IS SUPER INCONSISTENT)
// (ALL OF IT IS SUPER INCONSISTENT; IDK IF IT'S MY PART OR WHAT)
RotaryEncoder PollRotaryEnc() {
  rotEncCurPos += GetRotaryKnobDir(&rotEncState, ROTARYENCPINA, ROTARYENCPINB);
  int8_t dir = (rotEncOldPos - rotEncCurPos);
  // if at rest pos and the direction total is greater than a 4 state dif in one direction
  if ( (rotEncState == 3) && ( abs(dir) >= 4) ) {
    rotEncOldPos = rotEncCurPos;
    return RotaryEncoder(dir>>2);
  }
  /*
  if ( (rotEncClk != digitalRead(ROTARYENCCLK)) && (rotEncClk == HIGH) ) {
    rotEncClk = digitalRead(ROTARYENCCLK);
    return clk;
  }
  */
  return RotaryEncoder(0);
}

// ---- UTILITY FUNCTIONS ---- //

vec2 GetJoystickAxes(const uint8_t PinA, const uint8_t PinB) {
  uint16_t joyX = analogRead(PinA);
  uint16_t joyY = analogRead(PinB);
  return vec2(joyX,joyY);
}

bool OLEDGetPixel(Adafruit_SSD1306 display, vec2 v) {
  return display.getPixel(v.x, v.y);
}

// Returns time in milliseconds since last frame
unsigned long MillisToFrameTime(unsigned long time0, unsigned long time1) { return (time1-time0); }

// Converts the EmulatorState enum to a capitalized String
String EmuStateToString (EmulatorState state) {
  switch (state) {
    case (snake):
    return "Snake";
    case (pong):
    return "Pong";
    case (tron):
    return "Tron";
    case (doom):
    return "Doom";
    default:
    return "Selection";
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

// Draws the next frame of a monochrome Adafruit SSD1306 OLED display
// - Takes the input of the Adafruit_SSD1306 object, an integer number of
// - pixels to be drawn, and variable number of vec2 objects (max of 65535)
void DrawNextFrame(Adafruit_SSD1306 display, uint16_t pNum, vec2 pixel, ...) {
  va_list list;
  va_start(list, pixel);

  for (uint16_t i = 0; i < pNum; i++) {
    vec2 p = va_arg(list, vec2);
    // draw pixels into buffer
    display.drawPixel(p.x, p.y, 1);
  }

  va_end(list);
  // display pixels in buffer
  display.display();
}

// Prints Strings to an input LCD display and adjusts the text positioning based on an input LCDText data structure
// - Text that is longer than the column number will be cut off (limited to a max of 128 columns)
// - Any args provided that surpass the row number will not be used (limited to a max of 128 rows)
// - Providing less args than are columns will result in undefined behaviour 
void LCDPrint(LiquidCrystal LCD, LCDText text, ...) {
  LCD.clear();
  // prepare list of all args
  va_list list;
  va_start(list, text);

  for (uint8_t i = 0; i < LCDROWS; i++) {
    // fetchs current and moves to next LCDText object
    LCDText t = va_arg(list, LCDText);

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

  }

  va_end(list);
}
