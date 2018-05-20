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

struct Drawable
{
  GLuint vao;
  GLsizei indexCount;
};

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

GLuint compileShader(const char* shaderString, GLenum shaderType)
{
  GLuint shaderID = glCreateShader(shaderType);
  const GLchar* shaderSource = (const GLchar*)shaderString;
  glShaderSource(shaderID, 1, &shaderSource, NULL);
  glCompileShader(shaderID);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    printf("Shader compilation failed: \n%s\n", infoLog);
    return 0;
  }
  return shaderID;
}

GLuint linkShader(GLuint vShaderID, GLuint fShaderID)
{
  GLuint shader = glCreateProgram();
  glAttachShader(shader, vShaderID);
  glAttachShader(shader, fShaderID);
  glLinkProgram(shader);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(shader, 512, NULL, infoLog);
    printf("Shader linking failed: \n%s\n", infoLog);
    return -1;
  }
  return shader;
}

GLuint loadShader()
{
  GLuint vShaderID = compileShader(vShader, GL_VERTEX_SHADER);
  GLuint fShaderID = compileShader(fShader, GL_FRAGMENT_SHADER);
  assert(vShaderID && fShaderID);

  GLuint shader = linkShader(vShaderID, fShaderID);

  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);
  return shader;
}

GLuint bufferVertexData(GLfloat*  positions,  GLsizei positionBufferSize,
                        GLfloat*  colours,    GLsizei colourBufferSize,
                        GLuint*   indices,    GLsizei indexBufferSize)
{
  GLuint VBO[2], VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(2, VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, positionBufferSize, positions, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, colourBufferSize, colours, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STATIC_DRAW);
  return VAO;
}

Drawable loadRect()
{
    GLfloat positions[] =
    {
      -0.5f, 0.5f,
       0.5f, 0.5f,
       0.5f, -0.5f,
      -0.5f, -0.5f
    };

    GLfloat colours[] =
    {
      0.367f, 0.281f, 0.102f,
      0.367f, 0.281f, 0.102f,
      0.367f, 0.281f, 0.102f,
      0.367f, 0.281f, 0.102f
    };
    GLuint indices[] = {0, 1, 2, 0, 2, 3};
    GLsizei positionBufferSize = sizeof(positions);
    GLsizei colourBufferSize = sizeof(colours);
    GLsizei indexBufferSize = sizeof(indices);
    GLuint vao = bufferVertexData(positions, positionBufferSize, colours, colourBufferSize,
                                  indices, indexBufferSize);
    GLsizei indexCount = 6;
    Drawable rect = {vao, indexCount};
    return rect;
}

Drawable loadLine()
{
  GLfloat positions[] =
  {
    0.75f, 0.75f,
    0.75f, -0.75f
  };

  GLfloat colours[] =
  {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
  };
  GLuint indices[] = {0, 1};
  GLsizei positionBufferSize = sizeof(positions);
  GLsizei colourBufferSize = sizeof(colours);
  GLsizei indexBufferSize = sizeof(indices);
  GLuint vao = bufferVertexData(positions, positionBufferSize, colours, colourBufferSize,
                                indices, indexBufferSize);
  GLsizei indexCount = 2;
  Drawable line = {vao, indexCount};
  return line;
}

void draw(Drawable d, GLenum drawMode)
{
  glBindVertexArray(d.vao);
  glDrawElements(drawMode, d.indexCount, GL_UNSIGNED_INT, 0);
}

int main(int argc, char** argv)
{
  SFML_Window window(960, 540, "Pong");
  byte* windowBuffer = new byte[sizeof(sf::Window)];
  window.init(windowBuffer);
  LibInit::initGLEW();
  float black[] = {0.0f, 0.0f, 0.0f};
  window.setClearColour(black);

  GLuint shader = loadShader();
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
      draw(rect, GL_TRIANGLES);
      draw(line, GL_LINES);
      window.update();
    }
  }

  delete[] windowBuffer;
  return 0;
}
