#include "Pong.hpp"

void setInitialConditions(EntityList* entities)
{
  	Vec2f ballPosition = {COURT_WIDTH/2.0f, COURT_HEIGHT/2.0f};
	setEntityPosition(entities, BALL, ballPosition);
	setEntityVelocity(entities, BALL, Vec2f{0.0f, 0.0f});
}

void initEntity(EntityList* entities, uint entityType, Vec2f position, Vec2f size, uint index)
{
  assert(index < ENTITY_COUNT);
  createEntity(entities, entityType, size, position, index);
}

void initEntities(EntityList* entities, Vec2f paddleSize, Vec2f ballSize)
{
	//Init paddles
	Vec2f paddle1Position = {PADDLE_X, COURT_HEIGHT/2.0f};
	Vec2f paddle2Position = {COURT_WIDTH - PADDLE_X, COURT_HEIGHT/2.0f};
	initEntity(entities, ENTITY_PADDLE, paddle1Position, paddleSize, PADDLE_LEFT);
	initEntity(entities, ENTITY_PADDLE, paddle2Position, paddleSize, PADDLE_RIGHT);
	
	//Init ball
  	Vec2f ballPosition = {COURT_WIDTH/2.0f, COURT_HEIGHT/2.0f};
  	initEntity(entities, ENTITY_BALL, ballPosition, ballSize, BALL);
	
	//Init walls
	Vec2f horizontalWallSize = {COURT_WIDTH, WALL_WIDTH};
	Vec2f verticalWallSize = {WALL_WIDTH, COURT_HEIGHT};
	Vec2f topWallPosition = {COURT_WIDTH/2.0f, -0.5*WALL_WIDTH};
	Vec2f bottomWallPosition = {COURT_WIDTH/2.0f, COURT_HEIGHT + 0.5f*WALL_WIDTH};
	Vec2f leftWallPosition = {-0.5f*WALL_WIDTH, COURT_HEIGHT/2.0f};
	Vec2f rightWallPosition = {COURT_WIDTH + 0.5f*WALL_WIDTH, COURT_HEIGHT/2.0f};
	initEntity(entities, ENTITY_WALL, topWallPosition, horizontalWallSize, TOP_WALL);
	initEntity(entities, ENTITY_WALL, bottomWallPosition, horizontalWallSize, BOTTOM_WALL);
	initEntity(entities, ENTITY_GOAL, leftWallPosition, verticalWallSize, LEFT_GOAL);
	initEntity(entities, ENTITY_GOAL, rightWallPosition, verticalWallSize, RIGHT_GOAL);
}

GameState* initGame(Platform* platform)
{
  	GameState* gameState = (GameState*)allocate(platform, sizeof(GameState));
  
  	initConfig(&(gameState->config));
  
	Vec2f netSize = {0.0f, COURT_HEIGHT};

  	initGraphicalData(platform, &(gameState->graphicalData), gameState->config.paddleSize, gameState->config.ballSize, netSize);
  
  	initEntities(&(gameState->entities), gameState->config.paddleSize, gameState->config.ballSize);
  
  	initGameData(&(gameState->gameData));
  
  	initControllers(&(gameState->controllers));

  	initEventQueue(&(gameState->events));  
  
  	return gameState;
}

void handlePaddleController(Platform* platform, EntityList* entities, Controller* controller, uint entity, float paddleSpeed)
{
	Vec2f entityVelocity = getEntityVelocity(entities, entity);
	if(isUpPressed(platform, controller)) entityVelocity.y = - paddleSpeed;
	if(isDownPressed(platform, controller)) entityVelocity.y = paddleSpeed;
	setEntityVelocity(entities, entity, entityVelocity);
}

void serve(EntityList* entities, GameData* gameData, GameConfig* config)
{
	float theta = PI/4;
	float x = config->ballSpeed * cos(theta);
	float y = config->ballSpeed * sin(theta);
	Vec2f ballVelocity = {x, y};
	setEntityVelocity(entities, BALL, ballVelocity);
	startPlay(gameData);
}

void handleStartInput(Platform* platform, EntityList* entities, ControllerList* controllers, GameData* gameData, GameConfig* config)
{
  	if(isServePressed(platform, getController(controllers, PADDLE_LEFT)) || isServePressed(platform, getController(controllers, PADDLE_RIGHT))) serve(entities, gameData, config);
}

void handleControllerInput(Platform* platform, EntityList* entities, ControllerList* controllers, GameData* gameData, GameConfig* gameConfig)
{
	if(!gameData->isPlaying)
	{
		handleStartInput(platform, entities, controllers, gameData, gameConfig);
	}
	for(uint entity = PADDLE_LEFT; entity <= PADDLE_RIGHT; entity++)
	{
		Controller* paddleController = getController(controllers, entity);
		handlePaddleController(platform, entities, paddleController, entity, gameConfig->paddleSpeed);
	}
}


void handleBallCollision(EntityList* entities, uint collidingEntity, Vec2f collisionPoint, Vec2f collidingWall, Vec2f collidingNormal, Vec2f* p_1, Vec2f* entityVelocity, EventQueue* events)
{
	if(isRigidBody(entities, collidingEntity))
	{
		Vec2f d = *p_1 - collisionPoint;
		*p_1 = collisionPoint + projection(d, collidingWall) - projection(d, collidingNormal);
		*entityVelocity = projection(*entityVelocity, collidingWall) - projection(*entityVelocity, collidingNormal);

		pushEvent(events, EVENT_BALL_COLLIDE, 0); // Passes 0 as garbage
	}
	else if(isEntityType(entities, collidingEntity, ENTITY_GOAL))
	{
		pushEvent(events, EVENT_GOAL_SCORED, collidingEntity);
	}
}

void handlePaddleCollision(EntityList* entities, uint collidingEntity, Vec2f collisionPoint, Vec2f collidingNormal, Vec2f* p_1)
{
	if(isRigidBody(entities, collidingEntity))
	{
		*p_1 = collisionPoint + 0.0001f*collidingNormal;
	}
	else if(isEntityType(entities, collidingEntity, ENTITY_BALL))
	{
		//Move ball to edge of paddle
		Vec2f newBallPosition = getEntityPosition(entities, collidingEntity) + 2*(*p_1 - collisionPoint);
		setEntityPosition(entities, collidingEntity, newBallPosition);
	}
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
	
	//Check for collisions
	for(uint i = 0; i < ENTITY_COUNT; i++)
	{
	  	if(i != entity)
	    	{
	     	 	bool mayCollide = checkRectCollision(getEntityPosition(entities, i), getEntitySize(entities, i), p_0, p_1, entitySize, &collidingWall, &collidingNormal, &tMin);
	      		
			if(mayCollide) collidingEntity = i;
	      		collided = collided || mayCollide;
	    	}
	}
	
	//Calculate new position and velocity, push events to queue
	if(collided)
	{
		Vec2f collisionPoint = p_0 + tMin*(p_1 - p_0);
		if(isEntityType(entities, entity, ENTITY_BALL)) handleBallCollision(entities, collidingEntity, collisionPoint, collidingWall, collidingNormal, &p_1, &entityVelocity, events);
		else if(isEntityType(entities, entity, ENTITY_PADDLE)) handlePaddleCollision(entities, collidingEntity, collisionPoint, collidingNormal, &p_1);
	}

  	if(isEntityType(entities, entity, ENTITY_PADDLE)) entityVelocity = {0.0f, 0.0f};

  	setEntityPosition(entities, entity, p_1);
  	setEntityVelocity(entities, entity, entityVelocity/dt);
}

void moveEntities(EntityList* entities, float dt, GameData* gameData, EventQueue* events)
{
	for(uint entity = BALL; entity <= BALL; entity--) moveEntity(entities, entity, dt, gameData, events);
}


void processGoalScore(GameData* gameData, uint goal, EntityList* entities)
{
	uint scorer = (goal == LEFT_GOAL) ? PADDLE_RIGHT : PADDLE_LEFT;
	scoreGoal(gameData, scorer);
	setInitialConditions(entities);
	endPlay(gameData);
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
	if(windowWasResized(platform)) scaleGraphicalData(platform, &(gameState->graphicalData));
  	handleControllerInput(platform, &(gameState->entities), &(gameState->controllers), &(gameState->gameData), &(gameState->config));

  	moveEntities(&(gameState->entities), dt, &(gameState->gameData), &(gameState->events));

	handleEvents(&(gameState->events), &(gameState->gameData), &(gameState->entities), handleEvent);

  	drawEntities(&(gameState->entities), &(gameState->graphicalData));
	Vec2f netPos = {COURT_WIDTH/2.0f, 0.0f};
	drawNet(&(gameState->graphicalData), netPos);
}
