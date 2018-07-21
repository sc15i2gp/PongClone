#include "Config.hpp"

void initConfig(GameConfig* configuration)
{
  	configuration->ballSpeed = 500.0f;
 	configuration->paddleSpeed = 250.0f;
  	configuration->paddleSize= {20.0f, 100.0f};
  	configuration->ballSize = {20.0f, 20.0f};
}
