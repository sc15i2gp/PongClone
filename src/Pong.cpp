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

//Old paddle colour = {0.367f, 0.281f, 0.102f}

void initEntity(GameState* gameState, uint entityType, Vec2f position, Vec2f size, uint index)
{
  assert(index < ENTITY_COUNT);
  createEntity(&(gameState->entities), entityType, size, position, index);
}

void initPaddles(GameState* gameState)
{
  Vec2f paddleSize = {20.0f, 100.0f};
  Vec2f paddle1Position = {50.0f, 270.0f};
  Vec2f paddle2Position = {890.0f, 270.0f};
  initEntity(gameState, ENTITY_PADDLE, paddle1Position, paddleSize, PADDLE_LEFT);
  initEntity(gameState, ENTITY_PADDLE, paddle2Position, paddleSize, PADDLE_RIGHT);
  Drawable paddleDrawable = loadRect(paddleSize.x, paddleSize.y, 1.0f, 1.0f, 1.0f);
  gameState->drawables[PADDLE_DRAWABLE] = paddleDrawable;
  gameState->paddleSpeed = 250.0f;

}

void initBall(GameState* gameState)
{
  Vec2f ballSize = {20.0f, 20.0f};
  Vec2f ballPosition = {960.0f/2.0f, 540.0f/2.0f};
  initEntity(gameState, ENTITY_BALL, ballPosition, ballSize, BALL);
  Drawable ballDrawable = loadRect(ballSize.x, ballSize.y, 1.0f, 1.0f, 1.0f);
  gameState->drawables[BALL_DRAWABLE] = ballDrawable;
  gameState->ballSpeed = 500.0f;
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

bool checkCollisionWithBoundary(Vec2f wall_0, Vec2f wall_1, Vec2f p_0, Vec2f p_1, float* tMin)
{
  //Process + equations determined by calculating simultaneous equations results for vector intersection
  Vec2f dP = p_1 - p_0;
  Vec2f dW = wall_1 - wall_0;

  Vec2f a = wall_0 - p_0;
  float t = 2.0f; //Just something to be > 1.0f
  float r = 2.0f;

  if(dW.x == 0.0f)
  {
    if(dP.x != 0.0f)
    {
      t = a.x / dP.x;
      r = (t*dP.y - a.y) / dW.y;
    }
    else return false;
  }
  else if(dW.y == 0.0f)
  {
    if(dP.y != 0.0f)
    {
      t = a.y / dP.y;
      r  = (t*dP.x - a.x) / dW.x;
    }
    else return false;
  }
  else
  {
    float w = dW.x / dW.y;
    t = (a.x + a.y*w) / (dP.x - dP.y*w);
    r = (t*dP.y - a.y) / dW.y;
  }

  if(t >= 0.0f && t < *tMin)
  {
    if(r >= 0.0f && r <= 1.0f)
    {
      *tMin = t;
      return true;
    }
  }

  return false;
}

//Colliding entity is the entity which the moving entity will collide with
bool checkEntityCollision(GameState* gameState, uint collidingEntity, Vec2f p_0, Vec2f p_1,
                          Vec2f movingEntitySize, Vec2f* collidingWall, Vec2f* collidingNormal,
                          float* tMin)
{
  Vec2f collidingEntitySize = getEntitySize(&(gameState->entities), collidingEntity);
  Vec2f collidingEntityPosition = getEntityPosition(&(gameState->entities), collidingEntity);

  bool collided = false;
  Vec2f topLeftBound = collidingEntityPosition - 0.5f*collidingEntitySize - 0.5f*movingEntitySize;
  Vec2f bottomRightBound = collidingEntityPosition + 0.5f*collidingEntitySize + 0.5f*movingEntitySize;
  Vec2f wallPoints[] =
  {
    topLeftBound, Vec2f{bottomRightBound.x, topLeftBound.y}, //Top wall
    Vec2f{bottomRightBound.x, topLeftBound.y}, bottomRightBound, //Right wall
    bottomRightBound, Vec2f{topLeftBound.x, bottomRightBound.y}, //Bottom wall
    Vec2f{topLeftBound.x, bottomRightBound.y}, topLeftBound //Left wall
  };

  for(uint i = 0; i < 8; i += 2)
  {
    Vec2f wall_0 = wallPoints[i];
    Vec2f wall_1 = wallPoints[i+1];

    if(checkCollisionWithBoundary(wall_0, wall_1, p_0, p_1, tMin))
    {
      collided = true;
      *collidingWall = wall_1 - wall_0;
      *collidingNormal = normalise(Vec2f{collidingWall->y, -collidingWall->x});
    }
  }
  return collided;
}

bool isRigidBody(GameState* gameState, uint entity)
{
  return isEntityType(&(gameState->entities), entity, ENTITY_PADDLE) ||
         isEntityType(&(gameState->entities), entity, ENTITY_WALL);
}

void updateEntity(GameState* gameState, uint entity, float dt)
{
  Vec2f entityVelocity = dt*getEntityVelocity(&(gameState->entities), entity);
  Vec2f entitySize = getEntitySize(&(gameState->entities), entity);
  Vec2f p_0 = getEntityPosition(&(gameState->entities), entity);
  Vec2f p_1 = p_0 + entityVelocity;

  Vec2f collidingNormal;
  Vec2f collidingWall;
  float tMin = 1.0f;
  bool collided = false;
  uint collidingEntity;
  for(uint i = 0; i < ENTITY_COUNT; i++)
  {
    if(i != entity)
    {
      bool mayCollide = checkEntityCollision(gameState, i, p_0, p_1, entitySize, &collidingWall, &collidingNormal, &tMin);
      if(mayCollide) collidingEntity = i;
      collided = collided || mayCollide;
    }
  }
  if(collided)
  {
    Vec2f collisionPoint = p_0 + tMin*(p_1 - p_0);
    if(isEntityType(&(gameState->entities), entity, ENTITY_BALL))
    {
      if(isRigidBody(gameState, collidingEntity))
      {
        Vec2f d = p_1 - collisionPoint;
        p_1 = collisionPoint + projection(d, collidingWall) - projection(d, collidingNormal);
        entityVelocity = projection(entityVelocity, collidingWall) - projection(entityVelocity, collidingNormal);
      }
    }
    else if(isEntityType(&(gameState->entities), entity, ENTITY_PADDLE))
    {
      if(isRigidBody(gameState, collidingEntity))
      {
        p_1 = collisionPoint + collidingNormal;
      }
      else if(isEntityType(&(gameState->entities), collidingEntity, ENTITY_BALL))
      {
        //Move ball to edge of paddle
        Vec2f newBallPosition = getEntityPosition(&(gameState->entities), collidingEntity) +
                                2*(p_1 - collisionPoint);
        setEntityPosition(&(gameState->entities), collidingEntity, newBallPosition);
      }
    }
  }

  if(isEntityType(&(gameState->entities), entity, ENTITY_PADDLE)) entityVelocity = {0.0f, 0.0f};

  setEntityPosition(&(gameState->entities), entity, p_1);
  setEntityVelocity(&(gameState->entities), entity, entityVelocity/dt);
}

void handleControllerInput(Platform* platform, GameState* gameState)
{
  uint paddle1 = PADDLE_LEFT;
  uint ball = BALL;
  Vec2f paddle1Velocity = getEntityVelocity(&(gameState->entities), paddle1);
  Vec2f ballVelocity = getEntityVelocity(&(gameState->entities), ball);
  if(isKeyPressed(platform, Key::W))
  {
    paddle1Velocity.y = -gameState->paddleSpeed;
  }
  if(isKeyPressed(platform, Key::S))
  {
    paddle1Velocity.y = gameState->paddleSpeed;
  }
  if(isKeyPressed(platform, Key::Space))
  {
    float theta = PI/4;
    float x = gameState->ballSpeed * cos(theta);
    float y = gameState->ballSpeed * sin(theta);
    Vec2f ballVelocity = {x, y};
    setEntityVelocity(&(gameState->entities), ball, ballVelocity);
  }
  setEntityVelocity(&(gameState->entities), paddle1, paddle1Velocity);
}

void drawEntity(GameState* gameState, uint entityIndex, uint drawable)
{
  setScreenPosition(gameState, getEntityPosition(&(gameState->entities), entityIndex),
                    getEntitySize(&(gameState->entities), entityIndex));
  draw(gameState->drawables[drawable]);
}

//dt in seconds
void gameUpdate(Platform* platform, GameState* gameState, float dt)
{
  handleControllerInput(platform, gameState);

  updateEntity(gameState, BALL, dt);
  updateEntity(gameState, PADDLE_LEFT, dt);
  updateEntity(gameState, PADDLE_RIGHT, dt);

  useShader(gameState->shader);
  drawEntity(gameState, PADDLE_LEFT, PADDLE_DRAWABLE);
  drawEntity(gameState, PADDLE_RIGHT, PADDLE_DRAWABLE);
  drawEntity(gameState, BALL, BALL_DRAWABLE);
}
