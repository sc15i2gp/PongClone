#include <cstdio>
#include <assert.h>
#include <IPCommonUtils/Timer.hpp>
#include "Types.hpp"
#include "Platform.hpp"
#include "Pong.hpp"

//TODO: Add alignment stuff to setTranslationBlock

//TODO: FIX GOAL SCORING

//TODO: Make game screen size independent
//	Make sizes and velocities independent of screen size
//	Make court 24 x 18 units
//	Make ball 1/2 x 1/2 units
//	Make paddles 1/2 x 3 units
//	Make screen size 960 x 720 units
//	Make paddle position.x be 2.4 units in
//	Make ball position be game size / 2.0f
//	Make ball speed be about 13 units
//	Make paddle speed be 9 units 

//	Only need to change data in init functions
//	Pixel position = game position * (screen size/game size)
//	Uniform buffer object store projection matrix and ratio of screen to game size
//	Update ubo upon window resize

//TODO: Uniform buffer object for projection matrix and screen scale vector

//TODO: Change sizes used in game

//TODO: Window resize event handler




//TODO: Draw net
//	Draw dotted line
//	uniform buffer object for projection matrix
//	line shader (gShader to make net dashed line)
//	loadLine function
//	drawNet function

//TODO: Draw scores
//	FreeType lib used to create glyph texture atlas
//	Print scores as digit glyphs
//	(Future) When a player wins, display "LEFT/RIGHT PLAYER WINS"
//	(Future) Use letter glyphs for menu

//TODO: Menu
//	Play
//	Options
//	Quit
//	Have selected option render as red text
//	Wrap selection scroll

//TODO: Audio
//	OpenAL

//TODO: Particular paddle serves

//TODO: Win condition

//TODO: Modifiers
int main(int argc, char** argv)
{
  Platform* platform = initPlatform();
  GameState* gameState = initGame(platform);

  float black[] = {0.0f, 0.0f, 0.0f};
  setWindowClearColour(platform, black);

  bool running = true;
  Timer timer;
  float dt = 1.0f/60.0f;

  while(running)
  {
    float startTime = timer.getTimeRunning();
    pollInput(platform);

    if(shouldWindowClose(platform)) running = false;
    else
    {
      clearWindow(platform);
      gameUpdate(platform, gameState, dt);
      updateWindow(platform);
    }
    float endTime = timer.getTimeRunning();
    float timeRemaining = (dt*1000) - (endTime - startTime);
    if(timeRemaining > 0.0f) sleepFor(timeRemaining);
  }

  destroyPlatform(platform);
  return 0;
}
