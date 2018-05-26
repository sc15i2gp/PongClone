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

Entity createEntity(uint entityType, Vec2f size, Vec2f position)
{
  Entity e = {entityType, size, Vec2f{0.0f, 0.0f}, position};
  return e;
}

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
  Entity& entity = gameState->entities[BALL];
  Vec2f p_0 = entity.position;
  Vec2f p_1 = entity.position + entity.velocity;
  Vec2f velocity = entity.velocity;

  //Paddle check
  //Check paddle 1's right wall
  Entity paddle1 = gameState->entities[PADDLE_LEFT];
  float a = paddle1.position.x + (1.0f/2.0f)*paddle1.size.x + (1.0f/2.0f)*entity.size.x;
  float b_0 = paddle1.position.y - (1.0f/2.0f)*paddle1.size.y - (1.0f/2.0f)*entity.size.y;
  float b_1 = paddle1.position.y + (1.0f/2.0f)*paddle1.size.y + (1.0f/2.0f)*entity.size.y;
  float tMin = 1.0f;
  bool collided = checkCollisionWithBoundary(a, b_0, b_1, p_0.x, p_0.y, p_1.x, p_1.y, &tMin);
  if(collided)
  {
    Vec2f wallNormal = {1.0f, 0.0f};
    Vec2f collisionPoint = p_0 + tMin * velocity;
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

void handleControllerInput(Platform* platform, GameState* gameState)
{
  Entity& paddle1Entity = gameState->entities[PADDLE_LEFT];
  Entity& ballEntity = gameState->entities[BALL];
  if(isKeyPressed(platform, Key::W))
  {
    paddle1Entity.velocity.y = -5.0f;
  }
  if(isKeyPressed(platform, Key::S))
  {
    paddle1Entity.velocity.y = 5.0f;
  }
  if(isKeyPressed(platform, Key::Space))
  {
    ballEntity.velocity = {3.0f, 3.0f};
  }
}

void updateEntity(GameState* gameState, uint index)
{
  Entity& entity = gameState->entities[index];

  //Move entity and check for collision
    //Check for collision with outside walls
      //If collided with outside wall && entity is paddle
        //Put entity within bounds against wall
      //Else if collided with outside wall && entity is ball
        //If wall is top or bottom wall
          //Calculate ball's new position after collision
          //Reverse velocity in appropriate direction
        //Else if wall is left or right wall
          //Reset ball position and velocity
    //If entity is ball
      //Check for collisions with paddles
        //Use same method for each paddle wall as the outside walls
  //Draw entity
}

void updateEntities(GameState* gameState)
{
  for(uint i = 0; i < 3; i++)
  {
    updateEntity(gameState, i);
  }
}

void gameUpdate(Platform* platform, GameState* gameState)
{
  handleControllerInput(platform, gameState);

  updatePaddle(gameState->entities[PADDLE_LEFT]);
  updatePaddle(gameState->entities[PADDLE_RIGHT]);

  updateBall(gameState);

  //updateEntities(gameState);

  useShader(gameState->shader);
  setScreenPosition(gameState, gameState->entities[PADDLE_LEFT].position, gameState->entities[PADDLE_LEFT].size);
  draw(gameState->drawables[PADDLE_DRAWABLE]);
  setScreenPosition(gameState, gameState->entities[PADDLE_RIGHT].position, gameState->entities[PADDLE_RIGHT].size);
  draw(gameState->drawables[PADDLE_DRAWABLE]);
  setScreenPosition(gameState, gameState->entities[BALL].position, gameState->entities[BALL].size);
  draw(gameState->drawables[BALL_DRAWABLE]);
}
