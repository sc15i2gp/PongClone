#pragma once
#include <cmath>

struct Vec2f
{
  float x;
  float y;
};

Vec2f operator+(Vec2f v1, Vec2f v2);
Vec2f operator-(Vec2f v1, Vec2f v2);
Vec2f operator*(float f, Vec2f v);
Vec2f operator/(Vec2f v, float f);
void operator+=(Vec2f& v1, Vec2f& v2);
void operator-=(Vec2f& v1, Vec2f& v2);
void operator-=(Vec2f& v1, Vec2f&& v2);
float dot(Vec2f v1, Vec2f v2);
Vec2f projection(Vec2f v1, Vec2f v2); //v1 projected along v2
Vec2f normalise(Vec2f v);
float magnitude(Vec2f v);
