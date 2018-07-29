#include "GraphicalData.hpp"

const char vShader[] =
"#version 440 core\n"

"layout (location = 0) in vec2 modelPosition;\n"
"layout (location = 1) in vec3 inColour;\n"

"layout (std140, binding = 0) uniform Translation\n"
"{\n"
"mat4 projection;\n"
"mat2 scale;\n"
"};\n"

"out vec3 colour;\n"
"uniform vec2 position;\n"

"void main()\n"
"{\n"
"vec2 actualPosition = scale * (modelPosition + position);\n"
"gl_Position = projection * vec4(actualPosition, 0.0f, 1.0f);\n"
"colour = inColour;\n"
"}\0";

const char netVShader[] =
"#version 440 core\n"

"layout (location = 0) in vec2 modelPosition;\n"
"layout (location = 2) in float TexCoord;\n"
"layout (std140, binding = 0) uniform Translation\n"
"{\n"
"mat4 projection;\n"
"mat2 scale;\n"
"};\n"

"uniform vec2 position;\n"
"out float texCoord;\n"

"void main()\n"
"{\n"
"vec2 actualPosition = scale * (modelPosition + position);\n"
"gl_Position = projection * vec4(actualPosition, 0.0f, 1.0f);\n"
"texCoord = TexCoord;\n"
"}\0";

const char fShader[] =
"#version 440 core\n"
"in vec3 colour;\n"
"out vec4 fragColour;\n"
"void main()\n"
"{\n"
"fragColour = vec4(colour, 1.0f);\n"
"}\0";

const char netFShader[] = 
"#version 440 core\n"
"in float texCoord;\n"
"out vec4 fragColour;\n"
"uniform sampler1D net;\n"
"void main()\n"
"{\n"
"float c = texture1D(net, texCoord).x;\n"
"fragColour = vec4(c, c, c, 1.0f);\n"
"}\0";

void loadDrawable(Platform* platform, GraphicalData* graphicalData, uint index, Vec2f size, float R, float G, float B, Drawable (*loadFunc)(float, float, float, float, float))
{
	Drawable d = loadFunc(size.x, size.y, R, G, B);
	graphicalData->drawables[index] = d;
}

void initDrawables(GraphicalData* graphicalData, Vec2f ballSize, Vec2f paddleSize, Vec2f lineLength)
{
  	Drawable paddleDrawable = loadRect(paddleSize.x, paddleSize.y, 1.0f, 1.0f, 1.0f);
  	graphicalData->drawables[PADDLE_DRAWABLE] = paddleDrawable;
	
  	Drawable ballDrawable = loadRect(ballSize.x, ballSize.y, 1.0f, 1.0f, 1.0f);
  	graphicalData->drawables[BALL_DRAWABLE] = ballDrawable;

	Drawable netDrawable = loadLine(lineLength.x, lineLength.y, 1.0f, 1.0f, 1.0f);
	graphicalData->drawables[NET_DRAWABLE] = netDrawable;
}

void initShaders(GraphicalData* graphicalData)
{
	graphicalData->shaders[PB_SHADER] = loadShader(vShader, fShader);
	graphicalData->shaders[NET_SHADER] = loadShader(netVShader, netFShader);
}

void initUniformBuffer(Platform* platform, GraphicalData* graphicalData)
{
	graphicalData->translationUbo = createUniformBuffer(sizeof(glm::mat4) + sizeof(glm::mat4), 0);
	
	setTranslationBlock(platform, graphicalData->translationUbo, COURT_WIDTH, COURT_HEIGHT);
}

void initGraphicalData(Platform* platform, GraphicalData* graphicalData, Vec2f paddleSize, Vec2f ballSize, Vec2f lineLength)
{
	initDrawables(graphicalData, ballSize, paddleSize, lineLength);

	initShaders(graphicalData);

	initUniformBuffer(platform, graphicalData);

	float netTextureData[15] = {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
	graphicalData->netTexture = create1DTexture(netTextureData, 15);


	assert(graphicalData->shaders[0] && graphicalData->shaders[1] && graphicalData->translationUbo);
}

void scaleGraphicalData(Platform* platform, GraphicalData* graphicalData)
{
	setTranslationBlock(platform, graphicalData->translationUbo, COURT_WIDTH, COURT_HEIGHT);
}

void setScreenPosition(uint shader, Vec2f position, Vec2f size)
{
	position -= (0.5f*size);
  	setVec2Uniform(shader, "position", position);
}

void drawEntity(uint shader, Drawable drawable, Vec2f position, Vec2f size)
{
  	setScreenPosition(shader, position, size);
  	draw(drawable);
}

void drawEntities(EntityList* entities, GraphicalData* graphicalData)
{
  	useShader(graphicalData->shaders[PB_SHADER]);
	
	for(uint entity = PADDLE_LEFT; entity <= BALL; entity++)
	{
		Vec2f position = getEntityPosition(entities, entity);
		Vec2f size = getEntitySize(entities, entity);
		uint d = (isEntityType(entities, entity, ENTITY_PADDLE)) ? PADDLE_DRAWABLE : BALL_DRAWABLE;
		Drawable drawable = graphicalData->drawables[d];
		drawEntity(graphicalData->shaders[PB_SHADER], drawable, position, size);
	}
}

void drawNet(GraphicalData* data, Vec2f position)
{
	setLineWidth(3.0f);
	setVec2Uniform(data->shaders[NET_SHADER], "position", position);
	bindTexture(data->netTexture);
	draw(data->drawables[NET_DRAWABLE]);
	setLineWidth(1.0f);
}
