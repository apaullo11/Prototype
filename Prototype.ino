#include <LiquidCrystal.h>

#define LCDCOLUMNS 16
#define LCDROWS 2

LiquidCrystal lcd = LiquidCrystal(7, 6, 5, 4, 3, 2);
enum LCDAlignment { left, center, right };

struct LCDtext {
  LCDtext(String text, LCDAlignment align);
  String text;
  LCDAlignment align; 
};

enum EmuState { selection, snake, pong, doom };
enum GameStates { unactivated = 0, activated = 1, running = 2, failure = 4 };

EmuState emu = selection;
GameStates game1 = unactivated;
GameStates game2 = unactivated;
GameStates game3 = unactivated;

void setup() {
  // lcd init
  lcd.begin(LCDCOLUMNS, LCDROWS);
  lcd.noCursor();
  lcd.clear();

  // prevent main loop from starting if vars aren't initialized properly
  while (emu != selection || game1 != unactivated || game2 != unactivated || game3 != unactivated);
}

void loop() {
  // main game state 
  switch (emu) {
    case (selection):
      break;
    case (snake):
      break;
    case (pong):
      break;
    case (doom):
      break;
    default:
      break;
  }

}

// Writes input String a to column 1 and String b to column 2
void DisplayWrite(/*std::intializer_list<LCDtext> args*/) {
  /*
  lcd.setCursor(0,0);
  lcd.print();
  lcd.setCursor(0,1);
  lcd.print();
  */
}