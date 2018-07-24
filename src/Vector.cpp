#include "Vector.hpp"

Vec2f operator+(Vec2f v1, Vec2f v2)
{
  Vec2f v = {v1.x + v2.x, v1.y + v2.y};
  return v;
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

Vec2f operator/(Vec2f v, float f)
{
  Vec2f _v = {v.x/f, v.y/f};
  return _v;
}

void operator+=(Vec2f& v1, Vec2f& v2)
{
  v1.x += v2.x;
  v1.y += v2.y;
}

void operator-=(Vec2f& v1, Vec2f& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

void operator-=(Vec2f& v1, Vec2f&& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

float dot(Vec2f v1, Vec2f v2)
{
  return (v1.x*v2.x + v1.y*v2.y);
}

Vec2f projection(Vec2f v1, Vec2f v2)
{
  Vec2f u = normalise(v2);
  return dot(v1, u) * u;
}

Vec2f normalise(Vec2f v)
{
  return v/magnitude(v);
}

float magnitude(Vec2f v)
{
  return sqrt(pow(v.x, 2) + pow(v.y, 2));
}
