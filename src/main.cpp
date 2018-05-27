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

//TODO: Tidy up game code

      //Make it so that collision works with paddle moving opposite direction of ball and top/bottom wall collision
      //Possible approach:
          //If paddle will collide with ball
            //Adjust ball position to move outside of new position

//TODO: Fixed timestep
//TODO: Goal entity

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
