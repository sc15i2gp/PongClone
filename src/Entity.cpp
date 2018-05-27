#include "Entity.hpp"

void createEntity(EntityList* list, uint entityType, Vec2f size, Vec2f position, uint index)
{
  Entity e = {entityType, size, Vec2f{0.0f, 0.0f}, position};
  setEntity(list, e, index);
}

Vec2f getEntityPosition(EntityList* list, uint entity)
{
  return getEntity(list, entity)->position;
}

void setEntityPosition(EntityList* list, uint entity, Vec2f position)
{
  getEntity(list, entity)->position = position;
}

Vec2f getEntityVelocity(EntityList* list, uint entity)
{
  return getEntity(list, entity)->velocity;
}


void setEntityVelocity(EntityList* list, uint entity, Vec2f velocity)
{
  getEntity(list, entity)->velocity = velocity;
}


Vec2f getEntitySize(EntityList* list, uint entity)
{
  return getEntity(list, entity)->size;
}


void setEntitySize(EntityList* list, uint entity, Vec2f size)
{
  getEntity(list, entity)->size = size;
}


bool isEntityType(EntityList* list, uint entity, uint type)
{
  return getEntity(list, entity)->type == type;
}


Entity* getEntity(EntityList* list, uint index)
{
  assert(index < ENTITY_COUNT);
  return list->entities + index;
}

void setEntity(EntityList* list, Entity entity, uint index)
{
  assert(index < ENTITY_COUNT);
  list->entities[index] = entity;
}

uint entityCount(EntityList* list)
{
  return list->maxCount;
}
