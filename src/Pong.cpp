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

Entity createEntity(Vec2f size, Vec2f position)
{
  Entity e = {size, Vec2f{0.0f, 0.0f}, position};
  return e;
}

GameState* initGame(Platform* platform)
{
  GameState* gameState = (GameState*)allocate(platform, sizeof(GameState));
  Vec2f paddleSize = {20.0f, 100.0f};
  Vec2f paddle1Position = {50.0f, 270.0f};
  Vec2f paddle2Position = {890.0f, 270.0f};
  gameState->paddle1Entity = createEntity(paddleSize, paddle1Position);
  gameState->paddle2Entity = createEntity(paddleSize, paddle2Position);

  Vec2f ballSize = {20.0f, 20.0f};
  Vec2f ballPosition = {960.0f/2.0f, 540.0f/2.0f};
  gameState->ballEntity = createEntity(ballSize, ballPosition);

  gameState->paddle1 = loadRect(paddleSize.x, paddleSize.y, 0.367f, 0.281f, 0.102f);
  gameState->paddle2 = loadRect(paddleSize.x, paddleSize.y, 0.367f, 0.281f, 0.102f);
  gameState->ball = loadRect(ballSize.x, ballSize.y, 1.0f, 0.0f, 0.0f);

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

void updatePaddle(Entity& entity)
{
  entity.position += entity.velocity;
  if(entity.position.y < 0.0f + 0.5f*entity.size.y)
  {
    entity.position.y = 0.0f + 0.5f*entity.size.y;
  }
  else if(entity.position.y > 540.0f - 0.5f*entity.size.y)
  {
    entity.position.y = 540.0f - 0.5f*entity.size.y;
  }

  entity.velocity.y = 0.0f;
}

void updateBall(GameState* gameState)
{
  Entity& entity = gameState->ballEntity;
  Vec2f p_0 = entity.position;
  Vec2f p_1 = entity.position + entity.velocity;
  Vec2f velocity = entity.velocity;

  //Paddle check
  //Check paddle 1's right wall
  Entity paddle1 = gameState->paddle1Entity;
  float a = paddle1.position.x;
  float t = (a - p_0.x)/(p_1.x - p_0.x);
  bool collided = t <= 1.0f && t > 0.0f;
  if(collided)
  {

    Vec2f wallNormal = {1.0f, 0.0f};
    Vec2f collisionPoint = p_0 + t * velocity;
    Vec2f distanceRemaining = p_1 - collisionPoint;
    Vec2f newPosition = {collisionPoint.x - dot(distanceRemaining, wallNormal), p_1.y};
    entity.position = newPosition;
    entity.velocity.x = -entity.velocity.x;

  }
  else entity.position += entity.velocity;

  //Screen bounds check
  if(entity.position.y < 0.0f + 0.5f*entity.size.y)
  {
    entity.position.y = 0.0f + 0.5f*entity.size.y;
    entity.velocity.y = -entity.velocity.y;
  }
  else if(entity.position.y > 540.0f - 0.5f*entity.size.y)
  {
    entity.position.y = 540.0f - 0.5f*entity.size.y;
    entity.velocity.y = -entity.velocity.y;
  }
  if(entity.position.x < 0.0f + 0.5f*entity.size.x)
  {
    entity.position.x = 0.0f + 0.5f*entity.size.x;
    entity.velocity.x = -entity.velocity.x;
  }
  else if(entity.position.x > 960.0f - 0.5f*entity.size.x)
  {
    entity.position.x = 960.0f - 0.5f*entity.size.x;
    entity.velocity.x = -entity.velocity.x;
  }
}

void gameUpdate(Platform* platform, GameState* gameState)
{
  if(isKeyPressed(platform, Key::W))
  {
    gameState->paddle1Entity.velocity.y = -5.0f;
  }
  if(isKeyPressed(platform, Key::S))
  {
    gameState->paddle1Entity.velocity.y = 5.0f;
  }
  if(isKeyPressed(platform, Key::Space))
  {
    gameState->ballEntity.velocity = {3.0f, 3.0f};
  }

  updatePaddle(gameState->paddle1Entity);
  updatePaddle(gameState->paddle2Entity);

  //Update ball
  updateBall(gameState);

  useShader(gameState->shader);
  setScreenPosition(gameState, gameState->paddle1Entity.position, gameState->paddle1Entity.size);
  draw(gameState->paddle1);
  setScreenPosition(gameState, gameState->paddle2Entity.position, gameState->paddle2Entity.size);
  draw(gameState->paddle2);
  setScreenPosition(gameState, gameState->ballEntity.position, gameState->ballEntity.size);
  draw(gameState->ball);
}
