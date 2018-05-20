#pragma once

struct Vec2f
{
  float x;
  float y;
};

Vec2f operator+(Vec2f v1, Vec2f v2);
void operator+=(Vec2f& v1, Vec2f& v2);
