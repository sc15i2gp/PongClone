#include <cstdio>
#include <assert.h>
#include "SFML_Window.hpp"
#include "Types.hpp"

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
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
"}\0";

const char fShader[] =
"#version 410 core\n"
"out vec4 fragColour;\n"
"void main()\n"
"{\n"
"fragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";


int main(int argc, char** argv)
{
  SFML_Window window(960, 540, "Pong");
  byte* windowBuffer = new byte[sizeof(sf::Window)];
  window.init(windowBuffer);
  LibInit::initGLEW();
  float black[] = {0.0f, 0.0f, 0.0f};
  window.setClearColour(black);

  const GLchar* vShaderSource = (const GLchar*)vShader;
  const GLchar* fShaderSource = (const GLchar*)fShader;
  GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShaderID, 1, &vShaderSource, NULL);
  glCompileShader(vShaderID);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);
    printf("Vertex shader compile failed: \n%s\n", infoLog);
    return -1;
  }


  GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShaderID, 1, &fShaderSource, NULL);
  glCompileShader(fShaderID);
  glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    GLsizei length = 1000000;
    glGetShaderInfoLog(fShaderID, 512, &length, infoLog);
    printf("Fragment shader compile failed: %d\n%s\n", length, infoLog);
    return -1;
  }
  GLuint shader = glCreateProgram();
  glAttachShader(shader, vShaderID);
  glAttachShader(shader, fShaderID);
  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(shader, 512, NULL, infoLog);
    printf("Shader linking failed: \n%s\n", infoLog);
    return -1;
  }
  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);

  GLfloat positions[] =
  {
    -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };
  /*
  GLfloat colours[] =
  {
    0.367f, 0.281f, 0.102f,
    0.367f, 0.281f, 0.102f,
    0.367f, 0.281f, 0.102f,
    0.367f, 0.281f, 0.102f
  };*/
  GLuint indices[] = {0, 1, 2, 0, 2, 3};
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      window.update();
    }
  }

  delete[] windowBuffer;
  return 0;
}
