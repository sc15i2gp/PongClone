#pragma once
#include <assert.h>
#include "SFML_Window.hpp"

#define Bytes(n) n
#define Kilobytes(n) (1024*Bytes(n))

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


struct MemBuffer
{
  uint bufferSize = 0;
  uint inUse = 0;
  byte* buffer = NULL;
};


struct Platform
{
  MemBuffer* memBuffer;
  SFML_Window* window;
  Keyboard* keyboard;
};





/**     Platform      **/

Platform* initPlatform();
void destroyPlatform(Platform* platform);




/**     Memory      **/

MemBuffer* initMemory(uint memSize);
byte* allocate(Platform* platform, uint size);
byte* allocate(MemBuffer* memBuffer, uint size);

/**     Input     **/

Keyboard* initKeyboard(Platform* platform);
void pollInput(Platform* platform);
bool isKeyPressed(Platform* platform, Key key);




/**     window      **/

SFML_Window* initWindow(Platform* platform);
void setWindowClearColour(Platform* platform, float colour[3]);
bool shouldWindowClose(Platform* platform);
void clearWindow(Platform* platform);
void updateWindow(Platform* platform);


/**     Graphics      **/



GLuint loadShader(const char* vShader, const char* fShader);
Drawable loadRect();
Drawable loadLine();
void draw(Drawable d);
