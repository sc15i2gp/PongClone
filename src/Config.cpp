#include "Config.hpp"

void initConfig(GameConfig* configuration)
{
  	configuration->ballSpeed = 25.0f;
 	configuration->paddleSpeed = 40.0f;
  	configuration->paddleSize= {0.5f, 3.0f};
  	configuration->ballSize = {0.5f, 0.5f};
	printf("Paddle size: %.2f %.2f\n", configuration->paddleSize.x, configuration->paddleSize.y);
}
