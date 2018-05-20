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
  gameState->rect = loadRect(20.0f, 100.0f);
  gameState->rectPosition = {50.0f, 270.0f};
  gameState->shader = loadShader(vShader, fShader);
  setProjectionMatrix(platform, gameState->shader);
  return gameState;
}

void gameUpdate(Platform* platform, GameState* gameState)
{
  if(isKeyPressed(platform, Key::W))
  {
    gameState->rectPosition.y -= 5.0f;
    if(gameState->rectPosition.y < 0.0f) gameState->rectPosition.y = 0.0f;
  }
  if(isKeyPressed(platform, Key::S))
  {
    gameState->rectPosition.y += 5.0f;
    if(gameState->rectPosition.y > 540.0f - 100.0f) gameState->rectPosition.y = 540.0f - 100.0f;
  }

  setVec2Uniform(gameState->shader, "position", gameState->rectPosition);
  useShader(gameState->shader);
  draw(gameState->rect);
}
