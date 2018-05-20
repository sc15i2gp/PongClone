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

//TODO: Draw rectangle
      //Shader
        //Compile shader
        //Link shader
      //Buffer vertex data
      //Draw vertex data

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
  SFML_Window window(960, 540, "Pong");
  byte* windowBuffer = new byte[sizeof(sf::Window)];
  window.init(windowBuffer);
  LibInit::initGLEW();
  float black[] = {0.0f, 0.0f, 0.0f};
  window.setClearColour(black);

  GLuint shader = loadShader(vShader, fShader);
  Drawable rect = loadRect();
  Drawable line = loadLine();

  sf::Event e;
  bool running = true;

  while(running)
  {
    while(window.pollEvent(e))
    {
      switch(e.type)
      {
        case sf::Event::Closed:
          window.setToClose();
          break;
      }
    }

    if(window.shouldClose()) running = false;
    else
    {
      window.clear();
      glUseProgram(shader);
      draw(rect);
      draw(line);
      window.update();
    }
  }

  delete[] windowBuffer;
  return 0;
}
