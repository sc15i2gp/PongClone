#include "Pong.hpp"

//Old paddle colour = {0.367f, 0.281f, 0.102f}
void setInitialConditions(EntityList* entities)
{
  	Vec2f ballPosition = {960.0f/2.0f, 540.0f/2.0f};
	setEntityPosition(entities, BALL, ballPosition);
	setEntityVelocity(entities, BALL, Vec2f{0.0f, 0.0f});
}

void initControllers(Controls* controllers)
{
	controllers[PADDLE_LEFT] = {Key::W, Key::S};
	controllers[PADDLE_RIGHT] = {Key::I, Key::K};
}


void initEntity(EntityList* entities, uint entityType, Vec2f position, Vec2f size, uint index)
{
  assert(index < ENTITY_COUNT);
  createEntity(entities, entityType, size, position, index);
}

void initEntities(EntityList* entities, Vec2f paddleSize, Vec2f ballSize)
{
	//Init paddles
	Vec2f paddle1Position = {50.0f, 270.0f};
	Vec2f paddle2Position = {890.0f, 270.0f};
	initEntity(entities, ENTITY_PADDLE, paddle1Position, paddleSize, PADDLE_LEFT);
	initEntity(entities, ENTITY_PADDLE, paddle2Position, paddleSize, PADDLE_RIGHT);
	
	//Init ball
  	Vec2f ballPosition = {960.0f/2.0f, 540.0f/2.0f};
  	initEntity(entities, ENTITY_BALL, ballPosition, ballSize, BALL);
	
	//Init walls
	Vec2f horizontalWallSize = {960.0f, 2.0f};
	Vec2f verticalWallSize = {2.0f, 540.0f};
	Vec2f topWallPosition = {960.0f/2.0f, -2.0f};
	Vec2f bottomWallPosition = {960.0f/2.0f, 542.0f};
	Vec2f leftWallPosition = {-2.0f, 540.0f/2.0f};
	Vec2f rightWallPosition = {962.0f, 540.0f/2.0f};
	initEntity(entities, ENTITY_WALL, topWallPosition, horizontalWallSize, TOP_WALL);
	initEntity(entities, ENTITY_WALL, bottomWallPosition, horizontalWallSize, BOTTOM_WALL);
	initEntity(entities, ENTITY_GOAL, leftWallPosition, verticalWallSize, LEFT_GOAL);
	initEntity(entities, ENTITY_GOAL, rightWallPosition, verticalWallSize, RIGHT_GOAL);
}

void initConfiguration(GameConfig* configuration)
{
  	configuration->ballSpeed = 500.0f;
 	configuration->paddleSpeed = 250.0f;
  	configuration->paddleSize= {20.0f, 100.0f};
  	configuration->ballSize = {20.0f, 20.0f};
}

void initGameData(GameData* data)
{
	data->isPlaying = 0;
	for(uint i = 0; i < 2; i++) data->scores[i] = 0;
}

GameState* initGame(Platform* platform)
{
  GameState* gameState = (GameState*)allocate(platform, sizeof(GameState));
  
  initConfiguration(&(gameState->config));
  
  initGraphicalData(platform, &(gameState->graphicalData), gameState->config.paddleSize, gameState->config.ballSize);
  
  initEntities(&(gameState->entities), gameState->config.paddleSize, gameState->config.ballSize);
  
  initGameData(&(gameState->gameData));
  
  initControllers(gameState->controllers);

  initEventQueue(&(gameState->events));  
  
  return gameState;
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
bool checkEntityCollision(EntityList* entities, uint collidingEntity, Vec2f p_0, Vec2f p_1,
                          Vec2f movingEntitySize, Vec2f* collidingWall, Vec2f* collidingNormal,
                          float* tMin)
{
  Vec2f collidingEntitySize = getEntitySize(entities, collidingEntity);
  Vec2f collidingEntityPosition = getEntityPosition(entities, collidingEntity);

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

// Should only calculate the final positions of ball and paddles
void moveEntity(EntityList* entities, uint entity, float dt, GameData* gameData, EventQueue* events)
{
  	Vec2f entityVelocity = dt*getEntityVelocity(entities, entity);
  	Vec2f entitySize = getEntitySize(entities, entity);
  	Vec2f p_0 = getEntityPosition(entities, entity);
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
	     	 	bool mayCollide = checkEntityCollision(entities, i, p_0, p_1, entitySize, &collidingWall, &collidingNormal, &tMin);
	      		if(mayCollide) collidingEntity = i;
	      		collided = collided || mayCollide;
	    	}
	}
	if(collided)
	{
		Vec2f collisionPoint = p_0 + tMin*(p_1 - p_0);
		if(isEntityType(entities, entity, ENTITY_BALL))
		{
			if(isRigidBody(entities, collidingEntity))
			{
				Vec2f d = p_1 - collisionPoint;
				p_1 = collisionPoint + projection(d, collidingWall) - projection(d, collidingNormal);
				entityVelocity = projection(entityVelocity, collidingWall) - projection(entityVelocity, collidingNormal);
	
				pushEvent(events, EVENT_BALL_COLLIDE, 0); // Passes 0 as garbage
			}
			else if(isEntityType(entities, collidingEntity, ENTITY_GOAL))
			{
				pushEvent(events, EVENT_GOAL_SCORED, collidingEntity);
			}
		}
		else if(isEntityType(entities, entity, ENTITY_PADDLE))
		{
			if(isRigidBody(entities, collidingEntity))
			{
				p_1 = collisionPoint + collidingNormal;
			}
			else if(isEntityType(entities, collidingEntity, ENTITY_BALL))
			{
				//Move ball to edge of paddle
				Vec2f newBallPosition = getEntityPosition(entities, collidingEntity) + 2*(p_1 - collisionPoint);
				setEntityPosition(entities, collidingEntity, newBallPosition);
			}
		}
	}

  	if(isEntityType(entities, entity, ENTITY_PADDLE)) entityVelocity = {0.0f, 0.0f};

  	setEntityPosition(entities, entity, p_1);
  	setEntityVelocity(entities, entity, entityVelocity/dt);
}

void handlePaddleController(Platform* platform, EntityList* entities, Controls* controllers, uint entity, float paddleSpeed)
{
	Vec2f entityVelocity = getEntityVelocity(entities, entity);
	if(isKeyPressed(platform, controllers[entity].upKey)) entityVelocity.y = - paddleSpeed;
	if(isKeyPressed(platform, controllers[entity].downKey)) entityVelocity.y = paddleSpeed;
	setEntityVelocity(entities, entity, entityVelocity);
}

void handleStartInput(Platform* platform, EntityList* entities, float ballSpeed, byte* isPlaying)
{
  Vec2f ballVelocity = getEntityVelocity(entities, BALL);
  if(isKeyPressed(platform, Key::Space))
  {
    float theta = PI/4;
    float x = ballSpeed * cos(theta);
    float y = ballSpeed * sin(theta);
    Vec2f ballVelocity = {x, y};
    setEntityVelocity(entities, BALL, ballVelocity);
    *isPlaying = 1;
  }
}

void handleControllerInput(Platform* platform, EntityList* entities, Controls* controllers, GameData* gameData, GameConfig* gameConfig)
{
	if(!gameData->isPlaying)
	{
		handleStartInput(platform, entities, gameConfig->ballSpeed, &(gameData->isPlaying));
	}
	for(uint entity = PADDLE_LEFT; entity <= PADDLE_RIGHT; entity++)
	{
		handlePaddleController(platform, entities, controllers, entity, gameConfig->paddleSpeed);
	}
}

void moveEntities(EntityList* entities, float dt, GameData* gameData, EventQueue* events)
{
	for(uint entity = BALL; entity <= BALL; entity--) moveEntity(entities, entity, dt, gameData, events);
}


void processGoalScore(GameData* gameData, uint goal, EntityList* entities)
{
	uint i = (goal == LEFT_GOAL) ? PADDLE_RIGHT : PADDLE_LEFT;
	gameData->scores[i]++;
	printf("Score: { %d | %d }\n", gameData->scores[PADDLE_LEFT], gameData->scores[PADDLE_RIGHT]);
	setInitialConditions(entities);
	gameData->isPlaying = 0;
	// Play sound
}

void handleEvent(Event* e, GameData* gameData, EntityList* entities)
{
	switch(e->type)
	{
		case EVENT_GOAL_SCORED:
			processGoalScore(gameData, e->goal, entities);
			break;
		case EVENT_BALL_COLLIDE:
			// Play sound
			break;
	}
}

//dt in seconds
void gameUpdate(Platform* platform, GameState* gameState, float dt)
{
  	handleControllerInput(platform, &(gameState->entities), gameState->controllers, &(gameState->gameData), &(gameState->config));

  	moveEntities(&(gameState->entities), dt, &(gameState->gameData), &(gameState->events));

	handleEvents(&(gameState->events), &(gameState->gameData), &(gameState->entities), handleEvent);

  	drawEntities(&(gameState->entities), &(gameState->graphicalData));
}
