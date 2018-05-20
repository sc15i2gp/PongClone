#include "Vector.hpp"

Vec2f operator+(Vec2f v1, Vec2f v2)
{
  Vec2f v = {v1.x + v2.x, v1.y + v2.y};
  return v;
}

void operator+=(Vec2f& v1, Vec2f& v2)
{
  v1.x += v2.x;
  v1.y += v2.y;
}
