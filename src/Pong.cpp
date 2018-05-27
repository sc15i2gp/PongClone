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


void initEntity(GameState* gameState, uint entityType, Vec2f position, Vec2f size, uint index)
{
  assert(index < ENTITY_COUNT);
  gameState->entities[index] = createEntity(entityType, size, position);
}

void initPaddles(GameState* gameState)
{
  Vec2f paddleSize = {20.0f, 100.0f};
  Vec2f paddle1Position = {50.0f, 270.0f};
  Vec2f paddle2Position = {890.0f, 270.0f};
  initEntity(gameState, ENTITY_PADDLE, paddle1Position, paddleSize, PADDLE_LEFT);
  initEntity(gameState, ENTITY_PADDLE, paddle2Position, paddleSize, PADDLE_RIGHT);
  Drawable paddleDrawable = loadRect(paddleSize.x, paddleSize.y, 0.367f, 0.281f, 0.102f);
  gameState->drawables[PADDLE_DRAWABLE] = paddleDrawable;
}

void initBall(GameState* gameState)
{
  Vec2f ballSize = {20.0f, 20.0f};
  Vec2f ballPosition = {960.0f/2.0f, 540.0f/2.0f};
  initEntity(gameState, ENTITY_BALL, ballPosition, ballSize, BALL);
  Drawable ballDrawable = loadRect(ballSize.x, ballSize.y, 1.0f, 0.0f, 0.0f);
  gameState->drawables[BALL_DRAWABLE] = ballDrawable;
}

void initWalls(GameState* gameState)
{
  Vec2f horizontalWallSize = {960.0f, 2.0f};
  Vec2f verticalWallSize = {2.0f, 540.0f};
  Vec2f topWallPosition = {960.0f/2.0f, -2.0f};
  Vec2f bottomWallPosition = {960.0f/2.0f, 542.0f};
  Vec2f leftWallPosition = {-2.0f, 540.0f/2.0f};
  Vec2f rightWallPosition = {962.0f, 540.0f/2.0f};
  initEntity(gameState, ENTITY_WALL, topWallPosition, horizontalWallSize, TOP_WALL);
  initEntity(gameState, ENTITY_WALL, bottomWallPosition, horizontalWallSize, BOTTOM_WALL);
  initEntity(gameState, ENTITY_WALL, leftWallPosition, verticalWallSize, LEFT_WALL);
  initEntity(gameState, ENTITY_WALL, rightWallPosition, verticalWallSize, RIGHT_WALL);
}

GameState* initGame(Platform* platform)
{
  GameState* gameState = (GameState*)allocate(platform, sizeof(GameState));

  initPaddles(gameState);
  initBall(gameState);
  initWalls(gameState);

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

void updatePaddle(Entity* entity)
{
  Vec2f p = getEntityPosition(entity);
  Vec2f v = getEntityVelocity(entity);
  Vec2f s = getEntitySize(entity);
  p += v;
  if(p.y < 0.0f + 0.5f*s.y)
  {
    p.y = 0.0f + 0.5f*s.y;
  }
  else if(p.y > 540.0f - 0.5f*s.y)
  {
    p.y = 540.0f - 0.5f*s.y;
  }
  v.y = 0.0f;

  setEntityPosition(entity, p);
  setEntityVelocity(entity, v);
}

bool checkCollisionWithBoundary(float a, float b_0, float b_1, float p_0a, float p_0b,
                                float p_1a, float p_1b, float* tMin)
{
  float da = p_1a - p_0a;
  float db = p_1b - p_0b;
  float t = (a - p_0a)/(p_1a - p_0a);
  bool mayCollide = t > 0.0f && t <= *tMin;
  if(mayCollide)
  {
    float collisionPointb = p_0b + t*db;
    if(collisionPointb >= b_0 && collisionPointb <= b_1)
    {
      //Collided
      *tMin = t;
      return true;
    }
  }
  return false;
}

void updateBall(GameState* gameState)
{
  Entity* ball = gameState->entities + BALL;
  Vec2f ballVelocity = getEntityVelocity(ball);
  Vec2f ballSize = getEntitySize(ball);
  Vec2f p_0 = getEntityPosition(ball);
  Vec2f p_1 = p_0 + ballVelocity;

  //Paddle check
  //Check paddle 1's right wall
  Entity* paddle1 = gameState->entities + PADDLE_LEFT;
  Vec2f paddle1Size = getEntitySize(paddle1);
  Vec2f paddle1Position = getEntityPosition(paddle1);
  float a = paddle1Position.x + (1.0f/2.0f)*paddle1Size.x + (1.0f/2.0f)*ballSize.x;
  float b_0 = paddle1Position.y - (1.0f/2.0f)*paddle1Size.y - (1.0f/2.0f)*ballSize.y;
  float b_1 = paddle1Position.y + (1.0f/2.0f)*paddle1Size.y + (1.0f/2.0f)*ballSize.y;
  float tMin = 1.0f;
  bool collided = checkCollisionWithBoundary(a, b_0, b_1, p_0.x, p_0.y, p_1.x, p_1.y, &tMin);
  if(collided)
  {
    Vec2f wallNormal = {1.0f, 0.0f};
    Vec2f collisionPoint = p_0 + tMin * ballVelocity;
    Vec2f distanceRemaining = p_1 - collisionPoint;
    Vec2f newPosition = {collisionPoint.x - dot(distanceRemaining, wallNormal), p_1.y};
    p_1 = newPosition;
    ballVelocity.x = -ballVelocity.x;
  }

  //Screen bounds check
  if(p_1.y < 0.0f + 0.5f*ballSize.y)
  {
    p_1.y = 0.0f + 0.5f*ballSize.y;
    ballVelocity.y = -ballVelocity.y;
  }
  else if(p_1.y > 540.0f - 0.5f*ballSize.y)
  {
    p_1.y = 540.0f - 0.5f*ballSize.y;
    ballVelocity.y = -ballVelocity.y;
  }
  if(p_1.x < 0.0f + 0.5f*ballSize.x)
  {
    p_1.x = 0.0f + 0.5f*ballSize.x;
    ballVelocity.x = -ballVelocity.x;
  }
  else if(p_1.x > 960.0f - 0.5f*ballSize.x)
  {
    p_1.x = 960.0f - 0.5f*ballSize.x;
    ballVelocity.x = -ballVelocity.x;
  }
  setEntityPosition(ball, p_1);
  setEntityVelocity(ball, ballVelocity);
}

void handleControllerInput(Platform* platform, GameState* gameState)
{
  Entity* paddle1 = gameState->entities + PADDLE_LEFT;
  Entity* ball = gameState->entities + BALL;
  Vec2f paddle1Velocity = getEntityVelocity(paddle1);
  Vec2f ballVelocity = getEntityVelocity(ball);
  if(isKeyPressed(platform, Key::W))
  {
    paddle1Velocity.y = -5.0f;
  }
  if(isKeyPressed(platform, Key::S))
  {
    paddle1Velocity.y = 5.0f;
  }
  if(isKeyPressed(platform, Key::Space))
  {
    setEntityVelocity(ball, Vec2f{3.0f, 3.0f});
  }
  setEntityVelocity(paddle1, paddle1Velocity);
}

void drawEntity(GameState* gameState, uint entityIndex, uint drawable)
{
  Entity* e = gameState->entities + entityIndex;
  setScreenPosition(gameState, getEntityPosition(e), getEntitySize(e));
  draw(gameState->drawables[drawable]);
}

void gameUpdate(Platform* platform, GameState* gameState)
{
  handleControllerInput(platform, gameState);

  updatePaddle(gameState->entities + PADDLE_LEFT);
  updatePaddle(gameState->entities + PADDLE_RIGHT);

  updateBall(gameState);

  useShader(gameState->shader);
  drawEntity(gameState, PADDLE_LEFT, PADDLE_DRAWABLE);
  drawEntity(gameState, PADDLE_RIGHT, PADDLE_DRAWABLE);
  drawEntity(gameState, BALL, BALL_DRAWABLE);
}
