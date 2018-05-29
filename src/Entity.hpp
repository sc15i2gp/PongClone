#pragma once
#include <assert.h>
#include "Types.hpp"
#include "Vector.hpp"

#define ENTITY_COUNT 7

#define ENTITY_NULL 0
#define ENTITY_PADDLE (1 << 0)
#define ENTITY_BALL (1 << 1)
#define ENTITY_WALL (1 << 2)
#define ENTITY_GOAL (1 << 3)

struct Entity
{
  uint type = ENTITY_NULL;
  Vec2f size = {0.0f, 0.0f};
  Vec2f velocity = {0.0f, 0.0f};
  Vec2f position = {0.0f, 0.0f};
};

struct EntityList
{
  uint maxCount = ENTITY_COUNT;
  Entity entities[ENTITY_COUNT];
};

Vec2f getEntityPosition(EntityList* list, uint entity);
void setEntityPosition(EntityList* list, uint entity, Vec2f position);
Vec2f getEntityVelocity(EntityList* list, uint entity);
void setEntityVelocity(EntityList* list, uint entity, Vec2f velocity);
Vec2f getEntitySize(EntityList* list, uint entity);
void setEntitySize(EntityList* list, uint entity, Vec2f size);
bool isEntityType(EntityList* list, uint entity, uint type);

Entity* getEntity(EntityList* list, uint index);
void setEntity(EntityList* list, Entity entity, uint index);
uint entityCount(EntityList* list);
void createEntity(EntityList* list, uint entityType, Vec2f size, Vec2f position, uint index);
