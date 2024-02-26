//---- LIBRARIES AND INCLUDE FILES ----//
  #pragma once
  // Libraries
  #include <Adafruit_SSD1306.h>
  //#include <Adafruit_SSD1306_EMULATOR.h>
  #include <splash.h>
  //#include <LiquidCrystal.h>
  //#include <WString.h>

  #include "LinkedList.h"

  // BITMtextListS
  //#include "BitmtextLists/SnakeMenuPlay.h"
  //#include "BitmtextLists/SnakeMenuQuit.h"
//---- END OF LIBRARIES AND INCLUDE FILES ----//

//---- SCREEN DEFINITIONS ----//
  #define LCDCOLUMNS            16
  #define LCDROWS               2
  #define OLEDWIDTH             128
  #define OLEDHEIGHT            64
  #define OLEDLETTERW           6
  #define OLEDLETTERH           8
  // Play is centered roughly a fourth of the way across the screen, accounting for text size
  const uint8_t playButtonX0 = (OLEDWIDTH / 4) - (2 * 2 * OLEDLETTERW);
  const uint8_t playButtonX1 = (OLEDWIDTH / 4) + (2 * 2 * OLEDLETTERW);
  // Quit is centered roughly three fourths of the way across the screen, accounting for text size
  const uint8_t quitButtonX0 = (3 * OLEDWIDTH / 4) - (2 * 2 * OLEDLETTERW);
  const uint8_t quitButtonX1 = (3 * OLEDWIDTH / 4) + (2 * 2 * OLEDLETTERW);
  // Both Buttons are centered slightly above three fourths of the the way down the screen, accounting for text size
  const uint8_t menuButtonY = (3 * OLEDHEIGHT / 4) - (OLEDLETTERH + OLEDHEIGHT/32);
  const uint8_t menuSelectY = (3 * OLEDHEIGHT / 4) + (OLEDLETTERH);
//---- END OF SCREEN DFINITIONS ----//
//---- PIN DEFINITIONS ----//
  // LCD Display (ONLY 11-13 ARE USED DUE TO USING THE SHIFT REGISTER)
  /*
  #define LCDDS                 11
  #define LCDSHCP               13
  #define LCDSTCP               12
  #define RS                    11
  #define E                     3
  #define D4                    4
  #define D5                    5
  #define D6                    6
  #define D7                    7
  */
  // OLED Display (Definitions not used - just for reference)
  #define SDA                   A4  // Data
  #define SCL                   A5  // CLK
  // Interactive
  #define JOYSTICKXPIN          A0
  #define JOYSTICKYPIN          A1
  #define JOYSTICKCLK           7
  #define ROTARYENCPINA         9   // DT
  #define ROTARYENCPINB         10  // SW
  #define BUTTONPIN             8
  #define RGBPINR               3
  #define RGBPING               5
  #define RGBPINB               6
//---- END OF PIN DEFINITIONS ----//

//---- DATA STRUCTURING ----//
  // ENUM DEFS
  //enum LCDAlignment             { left, center, right };
  enum EmulatorState            { selection, snake, pong, tron, doom };
  enum GameState                { unactivated, activated, playing, failure = -1 };
  enum RotaryEncoder            { ccw = -1, none, cw, clk };
  /*
  struct LCDText {
    LCDText(String t, LCDAlignment a);
    // String to be printed to the LCD display
    String text;
    // Preferred alignment of the text on the LCD display (left by default)
    LCDAlignment align; 
    // Row String is to be printed to
    // int row = 0
  };
  */

  int rotEncOldPos = 0;
  int rotEncCurPos = 0;
  uint8_t rotEncState;
  bool buttonClk;

  //unsigned long t0 = millis();
  //unsigned long t1;
  unsigned long lastUpdate;
  float gameSpeed = 1.0;
  bool isPlaySelected = true;
//---- END OF DATA STRUCTURES ----//

Adafruit_SSD1306 OLED(OLEDWIDTH, OLEDHEIGHT);
//Adafruit_SSD1306_EMULATOR OLED(OLEDWIDTH, OLEDHEIGHT);
//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
//Adafruit_LiquidCrystal lcd(0);

enum EmulatorState emu = selection;
enum EmulatorState gameSelect = snake;

// I will make all these into a class for extensibility if I have the time
// - AKA class with base components and make objects or child classes with gamemode specific features and functions
enum GameState game1 = unactivated;
enum GameState game2 = unactivated;
enum GameState game3 = unactivated;
LinkedList *SnakeGame;
vec2 snakeFruit;
//unsigned int score;

void setup() {
  // random seed of unconnected analog input as recommended by Arduino documentation
  randomSeed(analogRead(A2));

  // PIN MODES
  //pinMode(LCDDS, OUTPUT);
  //pinMode(LCDSHCP, OUTPUT);
  //pinMode(LCDSTCP, OUTPUT);
  pinMode(ROTARYENCPINA, INPUT);
  pinMode(ROTARYENCPINB, INPUT);
  pinMode(BUTTONPIN, INPUT);

  Serial.begin(9600);
  /*
  // lcd init
  lcd.begin(LCDCOLUMNS, LCDROWS);
  lcd.noCursor();
  lcd.clear();
  */
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  OLED.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  OLED.clearDisplay();
  OLED.display();
  
  
  // prevent main loop from starting if state vars aren't initialized properly
  while (emu != selection || game1 != unactivated || game2 != unactivated || game3 != unactivated);

  rotEncState = GetRotaryState(ROTARYENCPINA, ROTARYENCPINB);
  buttonClk = digitalRead(BUTTONPIN);
  //t1 = millis();
  lastUpdate = 0;

  SnakeGame = new LinkedList();

  //LCDSelectGame(gameSelect);
  DrawGameOptions();
  OLEDSelectGame(gameSelect);
  RGBLED(RGBPINR, RGBPING, RGBPINB, 255, 0, 0);
}

void loop() {
  // main game states
  if (emu == selection) {
    // game selection state
    //Serial.println(String(emu));
    // get rotary encoder rotation
    enum RotaryEncoder rotEncInput = PollRotaryEnc();
    // unless button is pressed to select game
    // if button is down, AKA 1(TRUE) - set rotEncInput to 2, AKA clk
    //Serial.println(String(digitalRead(BUTTONPIN)));
    if (isButtonPressed(&buttonClk, BUTTONPIN)) rotEncInput = clk;
    switch (rotEncInput) {
      // Game is selected
      case (clk):
        emu = gameSelect;
        rotEncInput = none;
        //Serial.print(EmuStateToString(emu));
        //Serial.println(" Selected");
      break;

      case (none):
      break;

      // Rotary Encoder turned cw or ccw
      default:
        // EmulatorStates game states start at 1 and not 0 -> set to multiple of 3 before modulus and increment 1 after 
        gameSelect = ((gameSelect + 2 + rotEncInput) % 3) + 1;
        //LCDSelectGame(gameSelect);
        OLEDSelectGame(gameSelect);
        int r = 0, g = 0, b = 0;
        if (gameSelect == 1) r = 255;
        if (gameSelect == 2) g = 255;
        if (gameSelect == 3) b = 255;
        RGBLED(RGBPINR, RGBPING, RGBPINB, r, g, b);
      break;
    }
  }


    
  if (emu == snake) {
    //Serial.println("Emu is snake");
    switch (game1) {
      // boot game
      case (unactivated):
        BootGame(emu);
      break;

      // Menu Selection
      case (activated):
        GameMenuSelect();
        // if play, run game initalizer
        if (isButtonPressed(&buttonClk, BUTTONPIN)) {
          //Serial.println("Button Pressed");
          if (isPlaySelected) InitializeSnake();
          else QuitGame(&emu);
        }
      break;

        // game loop
        case (playing):
          // get next direction of snake
          *(SnakeGame->head->dir) = GetJoystickDir(JOYSTICKXPIN, JOYSTICKYPIN);
          // check if time for next frame
          if (tick()) {
            vec2 pos = SnakeNextPos();
            if (isSnakeDirValid(pos)) {
              SnakeNextFrame(pos);
            } else {
              GameOver();
            }
          }
        break;

      // game over
      case (failure):
        // remove linked list
      break;

      default:
        Serial.println("Game State invalid");
      break;
    }
  }



  if (emu == pong) {
    //Serial.println("Emu is pong");
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
  }



  if (emu == tron) {
    //Serial.println("Emu is tron");
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
  }
  //t0 = t1;
  //t1 = millis();
  //Serial.println(String(emu));
}

// ---- CONSTRUCTORS ---- //
/*
  LCDText::LCDText(String text, LCDAlignment align) { //, int row) {
    this->text = text;
    this->align = align;
    // this->row = row
  }
*/
// ---- END OF CONSTRUCTORS ---- //

/*
void LCDSelectGame(EmulatorState game) {
  LCDPrint(lcd, LCDText("Selected Game:", center), LCDText(EmuStateToString(game), center));
}
*/
void DrawGameOptions() {
  //Serial.println("Drawn");
  OLED.clearDisplay();
  OLED.setTextColor(1);
  OLED.setTextSize(2);
  for (int i = 1; i<=3; i++) {
    vec2 padding(0,0);
    padding.x = ( OLEDWIDTH - (2 * OLEDLETTERW * length(EmuStateToString(EmulatorState(i))) ) ) / 2;
    padding.y = ( (2*i - 1) * OLEDHEIGHT / 6) - (OLEDLETTERH);
    OLED.setCursor(padding.x, padding.y);
    OLED.print(EmuStateToString(EmulatorState(i)));
  }
  OLED.display();
}

void OLEDSelectGame(EmulatorState eState) {
  OLED.setTextSize(2);
  uint8_t x0, x1, y;
  // arbitrary number of letters selected for the middle word
  x0 = (( OLEDWIDTH - (2 * OLEDLETTERW * 8) ) / 2 );
  x1 = ( OLEDWIDTH + (2 * OLEDLETTERW * 6) ) / 2;
  for (uint8_t i = 1; i <= 3; i++) {
    y = ( (2*i - 1) * OLEDHEIGHT / 6) - (OLEDLETTERH);
    // if it is selected, the arrows are white - otherwise black/not visible
    if (i == eState) { OLED.setTextColor(1); }
    else OLED.setTextColor(0);

    OLED.setCursor(x0, y);
    OLED.write(0x10);
    OLED.setCursor(x1, y);
    OLED.write(0x11);
  }
  OLED.display();
}

// Boots up the game that the input EmulatorState is currently set to
void BootGame(EmulatorState game) {
  Serial.println(F("Game Booting!"));
  GameState *gState = EmuStateToGameState(game);
  // sets the GameState of that selected game to activated
  *(gState) = activated;
  //LCDPrint(lcd, LCDText("Running:", center), LCDText(EmuStateToString(game), center));
  Serial.println(EmuStateToString(game));
  // TODO - Draw logo? lol

  DrawGameMenu(game);
  
}

void DrawGameMenu(EmulatorState game) {
  OLED.clearDisplay();
  OLED.drawRect(0, 0, OLEDWIDTH, OLEDHEIGHT, 1);
  OLED.setTextColor(1);
  OLED.setTextSize(3);

  vec2 padding(0,0); 
  // center game title text horizontally
  padding.x = (OLEDWIDTH - (length((EmuStateToString(game))) * 3 * OLEDLETTERW)) / 2;
  // have bottom line of text on middle horizontal
  padding.y = (OLEDHEIGHT / 2) - (3 * OLEDLETTERH);
  
  OLED.setCursor(padding.x, padding.y);
  OLED.print(EmuStateToString(game));

  OLED.setTextSize(2);
  
  // play button
  padding.x = playButtonX0;
  padding.y = menuButtonY;
  OLED.setCursor(padding.x, padding.y);
  OLED.print(F("Play"));

  // quit button
  padding.x = quitButtonX0;
  // y coord is the same
  //padding[1] = (3 * OLEDHEIGHT / 4) - (OLEDLETTERH + OLEDHEIGHT/32);
  OLED.setCursor(padding.x, padding.y);
  OLED.print(F("Quit"));
  // FEATURE - Add Scoreboard?
  OLED.display();
}

void GameMenuSelect() {
  vec2 axisDirs = GetJoystickAxes(JOYSTICKXPIN,JOYSTICKYPIN);
  // play is selected
  if (axisDirs.x > (3*1023/5)) isPlaySelected = true;
  // quit is selected
  if (axisDirs.x < (2*1023/5)) isPlaySelected = false;
  DrawMenuLine(isPlaySelected);
}

// Draws line underneath the selected menu option
// - if input doDrawPlay is true, it will draw a white line underneath play and a black line underneath quit; vice versa if false
void DrawMenuLine(bool doDrawPlay) {
  uint8_t x0, x1, y;
  // play line
  x0 = playButtonX0 - 3;
  x1 = playButtonX1;
  y = menuSelectY;
  OLED.drawLine(x0, y, x1, y, doDrawPlay);
  // quit line
  x0 = quitButtonX0 - 3;
  x1 = quitButtonX1;
  OLED.drawLine(x0, y, x1, y, !doDrawPlay);
  OLED.display();
}

// Quits the selected game given an input 
// - sets the EmulatorState and GameState variables back to default states
void QuitGame(EmulatorState *emuState) {
  Serial.println(F("Quitting"));
  DrawGameOptions();
  OLEDSelectGame(gameSelect);
  // Sets GameState variable to unactivated
  (*EmuStateToGameState(*emuState)) = unactivated;
  // resets given emulator state
  *emuState = selection;
}

void GameOver() {
}

// Takes in an emulator state and returns a pointer to the respective GameState variable
GameState* EmuStateToGameState(EmulatorState emuState) {
  if (emuState == snake) {
    return &game1;
  } else if (emuState == pong) {
    return &game2;
  } else if (emuState == tron) {
    return &game3;
  } else if (emuState == doom) {
    //Serial.println("EmuState is DOOM");
  } else {
    //Serial.println("EmuState is null");
    return nullptr;
  }
}

// prepares snake for gamemode
void InitializeSnake() {
  lastUpdate = 0;
  Serial.print(F("Initial Size: "));
  Serial.println(SnakeGame->size);
  for (uint8_t i = 0; i<4; i++) {
    // add nodes to snake
    SnakeGame->addTailNode(vec2(OLEDWIDTH/2,(OLEDHEIGHT/2)+i), vec2(0,-1));
    //Serial.println("Node Created");
    Serial.println(SnakeGame->size);
  } 

  snakeFruit.x = random(0+3,OLEDWIDTH-3);
  snakeFruit.y = random(0+3,OLEDHEIGHT-3);

  OLED.fillRect(1, 1, 126, 62, 0);
  
  Serial.print(F("Snake Size: "));
  Serial.println(SnakeGame->size);
  Node* n = SnakeGame->head;
  for (uint8_t i = 0; i < SnakeGame->size; i++) {
    DrawPixel(OLED, *(n->pos), 1);
    n = n->prev;
    Serial.println(F("Node Drawn"));
  }

  //game1 = playing;
}

vec2 SnakeNextPos() {
  // direction of previous body node
  vec2 bodyDir = *(SnakeGame->head->prev->pos) - *(SnakeGame->head->pos);
  if (((*(SnakeGame->head->dir) + bodyDir).x == 0) && ((*(SnakeGame->head->dir) + bodyDir).y == 0)) return (bodyDir * -1);
  else return ((*(SnakeGame->head->dir) + bodyDir) / 2);
}

//
bool isSnakeDirValid(vec2 newPos) {
  Node* snakeNode = SnakeGame->head->prev;
  for (uint8_t i = 1; i < SnakeGame->size; i++) {
    if (*(snakeNode->pos) == newPos) {
      return false;
    }
    snakeNode = snakeNode->prev;
  }
  return true;
}


// Uses internal millisecond tracking to check if it is time for the next tick in game logic
bool tick() {
  unsigned int frameTime = 200 / gameSpeed;
  if ((millis() - lastUpdate) >= frameTime) {
    lastUpdate = millis();
    return true;
  }
}

// Compute logic for next frame; this function does these things:
// - Change positions of nodes
// - Draws and erases pixels
void SnakeNextFrame(vec2 pos) {
  if (pos == snakeFruit) {
    SnakeGame->addHeadNode(pos, *(SnakeGame->head->dir));
    snakeFruit.x = random(0+3,OLEDWIDTH-3);
    snakeFruit.y = random(0+3,OLEDHEIGHT-3);
  } else {
    DrawPixel(OLED, *(SnakeGame->tail->pos), 0);
    SnakeGame->moveBackToFront();
    DrawPixel(OLED, *(SnakeGame->head->pos), 1);
  }

  OLED.display();
}

// Polls the Rotary Encoder for changes in position or presses
// - Rotating takes precedent over clicking (NVM CLICKING IS SUPER INCONSISTENT)
// (ALL OF IT IS SUPER INCONSISTENT; IDK IF IT'S MY PART OR WHAT)
RotaryEncoder PollRotaryEnc() {
  rotEncCurPos += GetRotaryKnobDir(&rotEncState, ROTARYENCPINA, ROTARYENCPINB);
  //Serial.println(String(rotEncCurPos));
  int8_t dir = (rotEncOldPos - rotEncCurPos);
  // if at rest pos and the direction total is greater than a 4 state dif in one direction
  if ( (rotEncState == 3) && ( abs(dir) >= 4) ) {
    rotEncOldPos = rotEncCurPos;
    //Serial.println(String(dir>>2));
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

void RGBLED(uint16_t RPin, uint16_t GPin, uint16_t BPin, uint16_t Rval, uint16_t Gval, uint16_t Bval) {
  analogWrite(RPin,Rval);
  analogWrite(GPin,Gval);
  analogWrite(BPin,Bval);
}

inline unsigned int length(const char *c) {
  unsigned int size = 0;
  while(c[size] != '\0') size++;
  return size;
}

// Draws pixel into the buffer given an OLED display, vec2 position, and color
void DrawPixel(Adafruit_SSD1306 display, vec2 pos, uint16_t c) {
//void DrawPixel(Adafruit_SSD1306_EMULATOR display, vec2 pos, uint16_t c) {
  display.drawPixel(pos.x, pos.y, c);
}

bool isButtonPressed(bool* buttonState, uint8_t buttonPin) {
  uint8_t currentState = ((uint8_t)(*buttonState)<<1) | digitalRead(buttonPin);
  // Button isn't pressed and state is false - return 0
  if (currentState == 0) {
  }
  // Button is pressed, but state doesn't read it as so - return 1
  if (currentState == 1) {
    *buttonState = currentState;
  }
  // Button is not pressed, but state reads so - return 0
  if (currentState == 2) {
    *buttonState = 0;
  }
  // Button is pressed and state reads so - return 0
  if (currentState == 3) {
    return 0;
  } else {
    return (*buttonState);
  }
  
}

// Returns a vec2 of the analogRead inputs of the given pins
vec2 GetJoystickAxes(const uint8_t joystickXPin, const uint8_t joystickYPin) {
  uint16_t joyX = analogRead(joystickXPin);
  uint16_t joyY = analogRead(joystickYPin);
  return vec2(joyX,joyY);
}

// Return a vec2 of the current direction the analog stick is pointing in
vec2 GetJoystickDir(const uint8_t joystickXPin, const uint8_t joystickYPin) {
  vec2 axesVals = GetJoystickAxes(joystickXPin, joystickYPin);
  // I switched the physical pins since the joystick is rotated; as a result, the y-axis vals are flipped
  if (axesVals.x > 3*1023/5) axesVals.x = 1;
  if (axesVals.x < 2*1023/5) axesVals.x = -1;
  if (axesVals.y > 3*1023/5) axesVals.y = -1;
  if (axesVals.y < 2*1023/5) axesVals.y = 1;
  return axesVals;
}

// Returns time in milliseconds since last frame
//unsigned long MillisToFrameTime(unsigned long time0, unsigned long time1) { return (time1-time0); }

// Converts the EmulatorState enum to a String
const char* EmuStateToString (EmulatorState state) {
  const char *c;
  if (state == snake) {
    c = "Snake";
  } else if (state == pong) {
    c = "Pong";
  } else if (state == tron) {
    c = "Tron";
  } else if (state = doom) {
    c = "Doom";
  } else {
    c = "Selection";
  }
  return c;
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
  va_list vecList;
  va_start(vecList, pixel);

  for (uint16_t i = 0; i < pNum; i++) {
    vec2 p = va_arg(vecList, vec2);
    // draw pixels into buffer
    display.drawPixel(p.x, p.y, 1);
  }

  va_end(vecList);
  // display pixels in buffer
  display.display();
}

/*
// Prints Strings to an input LCD display and adjusts the text positioning based on an input LCDText data structure
// - Text that is longer than the column number will be cut off (limited to a max of 128 columns)
// - Any args provided that surpass the row number will not be used (limited to a max of 128 rows)
// - Providing less args than are columns will result in undefined behaviour 
void LCDPrint(LiquidCrystal LCD, ...) {
  LCD.clear();
  // prepare list of all args
  va_list textList;
  va_start(textList, LCD);
  
  for (uint8_t i = 0; i < LCDROWS; i++) {
    // fetchs current and moves to next LCDText object
    LCDText t = va_arg(textList, LCDText);

    // set text indent based on input alignment - if length is greater than columns, set to 0
    uint8_t indent = (LCDCOLUMNS < t.text.length()) ? 0 : (LCDCOLUMNS - t.text.length()) ; 
    if (t.align == left) {
      // no indent
      indent = 0;
    } else if (t.align == center) {
      // half of column difference to center text
      indent = indent/2;
    }
    // keep indent as column difference otherwise
    Serial.println(t.text);
    LCD.setCursor(indent,i);
    LCD.print(t.text.c_str());
  }
  va_end(textList);
}
*/
