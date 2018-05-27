#include "Entity.hpp"

Entity createEntity(uint entityType, Vec2f size, Vec2f position)
{
  Entity e = {entityType, size, Vec2f{0.0f, 0.0f}, position};
  return e;
}

Vec2f getEntityPosition(Entity* entity)
{
  return entity->position;
}

void setEntityPosition(Entity* entity, Vec2f position)
{
  entity->position = position;
}

Vec2f getEntityVelocity(Entity* entity)
{
  return entity->velocity;
}


void setEntityVelocity(Entity* entity, Vec2f velocity)
{
  entity->velocity = velocity;
}


Vec2f getEntitySize(Entity* entity)
{
  return entity->size;
}


void setEntitySize(Entity* entity, Vec2f size)
{
  entity->size = size;
}


bool isEntityType(Entity* entity, uint type)
{
  return entity->type == type;
}
