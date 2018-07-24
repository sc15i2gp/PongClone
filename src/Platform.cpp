#include "Platform.hpp"

Platform* initPlatform()
{
  	uint bytesToAllocate = Kilobytes(4);
  	MemBuffer* memBuffer = initMemory(bytesToAllocate);

	Platform* platform = (Platform*)allocate(memBuffer, sizeof(Platform));
 	platform->memBuffer = memBuffer;
	platform->window = initWindow(platform);
	platform->keyboard = initKeyboard(platform);
	return platform;
}

void destroyPlatform(Platform* platform)
{
  	MemBuffer* memBuffer = platform->memBuffer;
  	delete[] memBuffer;
}

void sleepFor(float milliseconds)
{
  	uint microseconds = (uint)(milliseconds*1000);
  	assert(usleep(microseconds) == 0);
}

void printMemBuffer(MemBuffer* buffer)
{
  	printf("MEMBUFFER\n");
  	printf("Location: %p\n", buffer);
  	printf("Size: %d\n", buffer->bufferSize);
  	printf("Bytes in use: %d\n", buffer->inUse);
  	printf("Buffer: %p\n", buffer->buffer);
  	printf("Next alloc location: %p\n", buffer->buffer + buffer->inUse);
}

MemBuffer* initMemory(uint memSize)
{
  	byte* buffer = new byte[memSize];
  	MemBuffer* memBuffer = new (buffer) MemBuffer;
  	memBuffer->bufferSize = Kilobytes(4) - (2*sizeof(uint));
  	memBuffer->inUse = 0;
  	memBuffer->buffer = buffer + sizeof(MemBuffer);
  	//printMemBuffer(memBuffer);
  	return memBuffer;
}

byte* allocate(MemBuffer* memBuffer, uint size)
{
  	assert(memBuffer->inUse + size <= memBuffer->bufferSize && "Attempted to allocate beyond buffer");
  	byte* ptr = memBuffer->buffer + memBuffer->inUse;
  	memBuffer->inUse += size;
  	return ptr;
}

byte* allocate(Platform* platform, uint size)
{
  	MemBuffer* memBuffer = platform->memBuffer;
  	return allocate(memBuffer, size);
}

Keyboard* initKeyboard(Platform* platform)
{
  	Keyboard* keyboard = (Keyboard*)allocate(platform, sizeof(Keyboard));
  	for(uint i = 0; i < Key::KeyCount; i++)
  	{
    		keyboard->isPressed[i] = false;
  	}
  	return keyboard;
}

void pollInput(Platform* platform)
{
  	SFML_Window* window = platform->window;
  	sf::Event e;
  	while(window->pollEvent(e))
  	{
    		switch(e.type)
    		{
      			case sf::Event::Closed:
        			window->setToClose();
        			break;
      			case sf::Event::KeyPressed:
        			platform->keyboard->isPressed[e.key.code] = true;
        			break;
      			case sf::Event::KeyReleased:
        			platform->keyboard->isPressed[e.key.code] = false;
        			break;
			case sf::Event::Resized:
				platform->window->resize(e.size.width, e.size.height);
				break;
    		}
  	}
}

bool isKeyPressed(Platform* platform, Key key)
{
  	return platform->keyboard->isPressed[key];
}

bool windowWasResized(Platform* platform)
{
	return platform->window->wasResized();
}

SFML_Window* initWindow(Platform* platform)
{
  	byte* windowBuffer = allocate(platform, sizeof(SFML_Window));
  	SFML_Window* window = new (windowBuffer) SFML_Window(960, 960, "Pong");
  	byte* sfWindowBuffer = allocate(platform, sizeof(sf::Window));
  	window->init(sfWindowBuffer);
  	LibInit::initGLEW();
  	return window;
}

void setWindowClearColour(Platform* platform, float colour[3])
{
  	platform->window->setClearColour(colour);
}

bool shouldWindowClose(Platform* platform)
{
  	return platform->window->shouldClose();
}

void clearWindow(Platform* platform)
{
  	platform->window->clear();
}

void updateWindow(Platform* platform)
{
  	platform->window->update();
}

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

uint loadShader(const char* vShader, const char* fShader)
{
  	GLuint vShaderID = compileShader(vShader, GL_VERTEX_SHADER);
  	GLuint fShaderID = compileShader(fShader, GL_FRAGMENT_SHADER);
  	assert(vShaderID && fShaderID);

  	GLuint shader = linkShader(vShaderID, fShaderID);

  	glDeleteShader(vShaderID);
  	glDeleteShader(fShaderID);
  	return (uint)shader;
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

Drawable loadRect(float width, float height, float R, float G, float B)
{
    	GLfloat positions[] =
    	{
      		0.0f, 	0.0f,
      		width, 	0.0f,
      		width, 	height,
      		0.0f, 	height
    	};

    	GLfloat colours[] =
    	{
      		R, G, B,
      		R, G, B,
      		R, G, B,
      		R, G, B
    	};

    	GLuint indices[] = {0, 1, 2, 0, 2, 3};
    	GLsizei positionBufferSize = sizeof(positions);
    	GLsizei colourBufferSize = sizeof(colours);
    	GLsizei indexBufferSize = sizeof(indices);
    	GLuint vao = bufferVertexData(positions, positionBufferSize, colours, colourBufferSize, indices, indexBufferSize);
    	GLsizei indexCount = 6;
    	Drawable rect = {vao, indexCount, GL_TRIANGLES};
    	return rect;
}

Drawable loadLine()
{
  	GLfloat positions[] =
  	{
    		0.75f, 	0.75f,
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
  	GLuint vao = bufferVertexData(positions, positionBufferSize, colours, colourBufferSize, indices, indexBufferSize);
  	GLsizei indexCount = 2;
  	Drawable line = {vao, indexCount, GL_LINES};
  	return line;
}

void draw(Drawable d)
{
  	glBindVertexArray(d.vao);
  	glDrawElements(d.drawMode, d.indexCount, GL_UNSIGNED_INT, 0);
}

void useShader(uint shader)
{
  	glUseProgram((GLuint)shader);
}

GLint uniformLocation(GLuint shader,
                      const char* uniformName)
{
  	return glGetUniformLocation(shader, uniformName);
}

void setMat4Uniform(GLuint shader, const char* uniform, glm::mat4 value)
{
  	glUseProgram(shader);
  	GLint location = uniformLocation(shader, uniform);
  	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
  	assert(glGetError() == GL_NO_ERROR);
}

void setProjectionMatrix(Platform* platform, uint shader)
{
 	float screenWidth = platform->window->getWidth();
  	float screenHeight = platform->window->getHeight();

  	glm::mat4 projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f);
  	setMat4Uniform((GLuint)shader, "projection", projection);
}

void setTranslationBlock(Platform* platform, uint ubo, float xUnitScale, float yUnitScale)
{
	float screenWidth = platform->window->getWidth();
	float screenHeight = platform->window->getHeight();

	glm::mat4 projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f);
	glm::mat4 scale = glm::mat4(0.0f);
	float scaleX = screenWidth/xUnitScale;
	float scaleY = screenHeight/yUnitScale;
	scale[0][0] = scaleX;
	scale[0][1] = 0.0f;
	scale[1][0] = 0.0f;
	scale[1][1] = scaleY;
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(scale));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		printf("%d error\n", error);
		assert(false);
	}
}

void setVec2Uniform(uint shader, const char* uniform, Vec2f value)
{
  	glUseProgram((GLuint)shader);
  	GLint location = uniformLocation(shader, uniform);
  	glUniform2f(location, value.x, value.y);
  	assert(glGetError() == GL_NO_ERROR);
}

uint createUniformBuffer(uint size, uint binding)
{
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	return ubo;
}
