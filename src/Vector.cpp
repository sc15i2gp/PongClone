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

Vec2f operator-(Vec2f v1, Vec2f v2)
{
  Vec2f v = {v1.x - v2.x, v1.y - v2.y};
  return v;
}

Vec2f operator*(float f, Vec2f v)
{
  Vec2f _v = {f*v.x, f*v.y};
  return _v;
}

float dot(Vec2f v1, Vec2f v2)
{
  return (v1.x*v2.x + v1.y*v2.y);
}
