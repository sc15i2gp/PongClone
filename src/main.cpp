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
      //Threading - NOT INITIALLY IMPLEMENTING (MIGHT NOT NEED AT ALL)

//TODO: Rectangle which can be moved
      //Give positions in pixels
      //Project matrix

const char vShader[] =
"#version 410 core\n"
"layout (location = 0) in vec2 modelPosition;\n"
"layout (location = 1) in vec3 inColour;\n"
"out vec3 colour;\n"
"uniform mat4 projection;\n"
"uniform vec2 position;\n"
"void main()\n"
"{\n"
"vec2 actualPosition = modelPosition + position;\n"
"gl_Position = projection * vec4(actualPosition, 0.0f, 1.0f);\n"
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

  uint shader = loadShader(vShader, fShader);
  Drawable rect = loadRect(20.0f, 100.0f);
  Vec2f rectPosition = {50.0f, 270.0f};

  setProjectionMatrix(platform, shader);

  bool running = true;

  while(running)
  {
    pollInput(platform);

    if(shouldWindowClose(platform)) running = false;
    else
    {
      clearWindow(platform);
      if(isKeyPressed(platform, Key::W))
      {
        rectPosition.y -= 5.0f;
        if(rectPosition.y < 0.0f) rectPosition.y = 0.0f;
      }
      if(isKeyPressed(platform, Key::S))
      {
        rectPosition.y += 5.0f;
        if(rectPosition.y > 540.0f - 100.0f) rectPosition.y = 540.0f - 100.0f;
      }

      setVec2Uniform(shader, "position", rectPosition);
      useShader(shader);
      draw(rect);

      updateWindow(platform);
    }
  }

  destroyPlatform(platform);
  return 0;
}
