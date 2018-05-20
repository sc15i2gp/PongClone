#include <cstdio>
#include <assert.h>
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

//TODO: Velocity for paddles + ball

int main(int argc, char** argv)
{
  Platform* platform = initPlatform();
  GameState* gameState = initGame(platform);

  float black[] = {0.0f, 0.0f, 0.0f};
  setWindowClearColour(platform, black);

  bool running = true;

  while(running)
  {
    pollInput(platform);

    if(shouldWindowClose(platform)) running = false;
    else
    {
      clearWindow(platform);
      gameUpdate(platform, gameState);
      updateWindow(platform);
    }
  }

  destroyPlatform(platform);
  return 0;
}
