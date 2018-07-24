#include <cstdio>
#include <assert.h>
#include <IPCommonUtils/Timer.hpp>
#include "Types.hpp"
#include "Platform.hpp"
#include "Pong.hpp"

//TODO: FIX GOAL SCORING

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
