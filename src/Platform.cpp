#include "Platform.hpp"

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

GLuint loadShader(const char* vShader, const char* fShader)
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
    Drawable rect = {vao, indexCount, GL_TRIANGLES};
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
  Drawable line = {vao, indexCount, GL_LINES};
  return line;
}

void draw(Drawable d)
{
  glBindVertexArray(d.vao);
  glDrawElements(d.drawMode, d.indexCount, GL_UNSIGNED_INT, 0);
}
