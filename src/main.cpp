#include <cstdio>
#include <assert.h>
#include "Types.hpp"
#include "Platform.hpp"

//NOTE: Platform provides environment for game to run in
      //Rendering
      //Input
      //Memory
      //Audio - NOT INITIALLY IMPLEMENTING
      //File I/O - NOT INITIALLY IMPLEMENTING
      //Threading - NOT INITIALLY IMPLEMENTING

//TODO: Memory buffer
      //Replace having to use new and delete
      //Allocate memory once, delete at the end
      
const char vShader[] =
"#version 410 core\n"
"layout (location = 0) in vec2 position;\n"
"layout (location = 1) in vec3 inColour;\n"
"out vec3 colour;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 0.0f, 1.0f);\n"
"colour = inColour;\n"
"}\0";

const char fShader[] =
"#version 410 core\n"
"in vec3 colour;\n"
"out vec4 fragColour;\n"
"void main()\n"
"{\n"
"fragColour = vec4(colour, 1.0f);\n"
"}\0";



int main(int argc, char** argv)
{
  Platform* platform = initPlatform();

  float black[] = {0.0f, 0.0f, 0.0f};
  setWindowClearColour(platform, black);

  GLuint shader = loadShader(vShader, fShader);
  Drawable rect = loadRect();
  Drawable line = loadLine();

  bool running = true;

  while(running)
  {
    pollInput(platform);

    if(shouldWindowClose(platform)) running = false;
    else
    {
      clearWindow(platform);

      glUseProgram(shader);
      draw(rect);
      draw(line);

      if(isKeyPressed(platform, Key::W)) printf("KEY W PRESSED\n");
      updateWindow(platform);
    }
  }

  destroyPlatform(platform);
  return 0;
}
