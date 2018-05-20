#pragma once
#include <assert.h>
#include "SFML_Window.hpp"

typedef sf::Keyboard::Key Key;

struct Keyboard
{
  bool isPressed[Key::KeyCount];
};

struct Drawable
{
  GLuint vao;
  GLsizei indexCount;
  GLenum drawMode;
};



struct Platform
{
  SFML_Window* window;
  Keyboard* keyboard;
};





/**     Platform      **/

Platform* initPlatform();
void destroyPlatform(Platform* platform);



/**     Input     **/

void pollInput(Platform* platform);





/**     window      **/

SFML_Window* initWindow();
void setWindowClearColour(Platform* platform, float colour[3]);
bool shouldWindowClose(Platform* platform);
void clearWindow(Platform* platform);
void updateWindow(Platform* platform);


/**     Graphics      **/



GLuint loadShader(const char* vShader, const char* fShader);
Drawable loadRect();
Drawable loadLine();
void draw(Drawable d);
