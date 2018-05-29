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

//TODO: Goal entity
      //Goal entity type
      //Goal entity collision
      //Reset ball on collision
      //Have entity hold reference to player entity (the one who gains a point when scoring in that goal)

      //TODO: "Round"
            //Gamestate has isPlaying or isServing etc.
            //If isServing, start game on space press
            //Else if isPlaying update entities and if ball collides with goal, reset game

//TODO: Scores
      //Player 1 and player 2 scores
      //Render scores

//TODO: Win condition
      //If a score > win score
          //Render "Left/right player wins"
          //Reset game scores
          //Reset game

//TODO: Audio
      //Play sound
      //Audio game data

//TODO: Config
      //File I/O
      //Game settings
      //Config file

//TODO: Pause

//TODO: Main menu

//TODO: Metrics gathering
      //Frame times
      //Memory usage (if possible)
      //Have thread dedicated to transforming logged metrics into graph data

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
