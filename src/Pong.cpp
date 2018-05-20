#include "Pong.hpp"

const char vShader[] =
"#version 410 core\n"
"layout (location = 0) in vec2 modelPosition;\n"
"layout (location = 1) in vec3 inColour;\n"
"out vec3 colour;\n"
"uniform mat4 projection;\n"
"uniform vec2 position;\n"
"void main()\n"
"{\n"
"vec2 actualPosition = modelPosition + position;\n"
"gl_Position = projection * vec4(actualPosition, 0.0f, 1.0f);\n"
"colour = inColour;\n"
"}\0";

const char fShader[] =
"#version 410 core\n"
"in vec3 colour;\n"
"out vec4 fragColour;\n"
"void main()\n"
"{\n"
"fragColour = vec4(colour, 1.0f);\n"
"}\0";


GameState* initGame(Platform* platform)
{
  GameState* gameState = (GameState*)allocate(platform, sizeof(GameState));
  gameState->paddleSize = {20.0f, 100.0f};
  gameState->ballSize = {20.0f, 20.0f};
  gameState->paddle1 = loadRect(gameState->paddleSize.x, gameState->paddleSize.y, 0.367f, 0.281f, 0.102f);
  gameState->paddle2 = loadRect(gameState->paddleSize.x, gameState->paddleSize.y, 0.367f, 0.281f, 0.102f);
  gameState->paddle1Position = {50.0f, 270.0f};
  gameState->paddle2Position = {890.0f, 270.0f};
  gameState->ball = loadRect(gameState->ballSize.x, gameState->ballSize.y, 1.0f, 0.0f, 0.0f);
  gameState->ballPosition = {960.0f/2.0f, 540.0f/2.0f};
  gameState->shader = loadShader(vShader, fShader);
  setProjectionMatrix(platform, gameState->shader);
  return gameState;
}

void setScreenPosition(GameState* gameState, Vec2f position, Vec2f size)
{
  position.x -= (1.0f/2.0f)*size.x;
  position.y -= (1.0f/2.0f)*size.y;
  setVec2Uniform(gameState->shader, "position", position);
}

void gameUpdate(Platform* platform, GameState* gameState)
{
  if(isKeyPressed(platform, Key::W))
  {
    gameState->paddle1Position.y -= 5.0f;
    if(gameState->paddle1Position.y < 0.0f) gameState->paddle1Position.y = 0.0f;
  }
  if(isKeyPressed(platform, Key::S))
  {
    gameState->paddle1Position.y += 5.0f;
    if(gameState->paddle1Position.y > 540.0f - 100.0f) gameState->paddle1Position.y = 540.0f - 100.0f;
  }

  useShader(gameState->shader);
  setScreenPosition(gameState, gameState->paddle1Position, gameState->paddleSize);
  draw(gameState->paddle1);
  setScreenPosition(gameState, gameState->paddle2Position, gameState->paddleSize);
  draw(gameState->paddle2);
  setScreenPosition(gameState, gameState->ballPosition, gameState->ballSize);
  draw(gameState->ball);
}
