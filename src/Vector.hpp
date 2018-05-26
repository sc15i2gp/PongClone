#pragma once

struct Vec2f
{
  float x;
  float y;
};

Vec2f operator+(Vec2f v1, Vec2f v2);
Vec2f operator-(Vec2f v1, Vec2f v2);
Vec2f operator*(float f, Vec2f v);
void operator+=(Vec2f& v1, Vec2f& v2);
float dot(Vec2f v1, Vec2f v2);
