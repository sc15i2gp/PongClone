#include <cstdio>
#include <assert.h>
#include <IPCommonUtils/Timer.hpp>
#include "Types.hpp"
#include "Platform.hpp"
#include "Pong.hpp"

//NOTE: Platform provides environment for game to run in
      //Rendering
      //Input
      //Memory
      //Audio - NOT INITIALLY IMPLEMENTING
      //File I/O - NOT INITIALLY IMPLEMENTING
      //Threading - NOT INITIALLY IMPLEMENTING (MIGHT NOT NEED AT ALL)

//TODO: Tidy up code

//TODO: Separate event code
//	Need function which hashes combination of entity types
//	Function should exist in entity code
//	Event code maps that hash code to functions

//TODO: Properly indent code
//TODO: Group game code properly

//TODO: Make game screen size independent

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
