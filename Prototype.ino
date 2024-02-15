#include <LiquidCrystal.h>
#define game1 snake
#define game2 pong
#define game3 doom

LiquidCrystal lcd = LiquidCrystal(7, 6, 5, 4, 3, 2);
enum MainLoop = { selection, game1, game2, game3 };
enum GameStates = { unactivated = 0, activated = 1, running = 2, failure = 4 };

MainLoop main = selection;
GameStates game1 = unactivated;
GameStates game2 = unactivated;
GameStates game3 = unactivated;

void setup() {
  lcd.begin(16, 2);

  // prevent main loop from starting if vars aren't initialized properly
  while (game1 != unactivated || game2 != unactivated || game3 != unactivated);
}

void loop() {
  // put your main code here, to run repeatedly:

}
