#pragma once
#include "Types.hpp"
#include "Vector.hpp"

#define ENTITY_NULL 0
#define ENTITY_PADDLE (1 << 0)
#define ENTITY_BALL (1 << 1)
#define ENTITY_WALL (1 << 2)

struct Entity
{
  uint type = ENTITY_NULL;
  Vec2f size = {0.0f, 0.0f};
  Vec2f velocity = {0.0f, 0.0f};
  Vec2f position = {0.0f, 0.0f};
};

Entity createEntity(uint entityType, Vec2f size, Vec2f position);
Vec2f getEntityPosition(Entity* entity);
void setEntityPosition(Entity* entity, Vec2f position);
Vec2f getEntityVelocity(Entity* entity);
void setEntityVelocity(Entity* entity, Vec2f velocity);
Vec2f getEntitySize(Entity* entity);
void setEntitySize(Entity* entity, Vec2f size);
bool isEntityType(Entity* entity, uint type);
