#pragma once
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <unistd.h>
#include "SFML_Window.hpp"
#include "Vector.hpp"

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
void sleepFor(float milliseconds);


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
bool windowWasResized(Platform*);




/**     Graphics      **/

uint loadShader(const char* vShader, const char* fShader);
Drawable loadRect(float width, float height, float R, float G, float B);
Drawable loadLine(float x, float y, float R, float G, float B);
void draw(Drawable d);
void useShader(uint shader);
void bindTexture(uint texture);
void setProjectionMatrix(Platform* platform, uint shader);
void setVec2Uniform(uint shader, const char* uniform, Vec2f value);
void setLineWidth(float width);
uint createUniformBuffer(uint size, uint binding);
void setTranslationBlock(Platform*, uint buffer, float xScale, float yScale);
uint create1DTexture(float* textureData, uint length);
