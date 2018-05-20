#pragma once
#include <assert.h>
#include "SFML_Window.hpp"

struct Drawable
{
  GLuint vao;
  GLsizei indexCount;
  GLenum drawMode;
};

GLuint loadShader(const char* vShader, const char* fShader);
Drawable loadRect();
Drawable loadLine();
void draw(Drawable d);
