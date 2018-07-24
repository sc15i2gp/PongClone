#include "GraphicalData.hpp"

const char vShader[] =
"#version 440 core\n"
"layout (location = 0) in vec2 modelPosition;\n"
"layout (location = 1) in vec3 inColour;\n"
"layout (std140, binding = 0) uniform Translation\n"
"{\n"
"mat4 p;\n"
"mat2 s;\n"
"};\n"
"out vec3 colour;\n"
"uniform vec2 position;\n"
"void main()\n"
"{\n"
"mat2 scale = s;\n"
"vec2 actualPosition = scale * (modelPosition + position);\n"
"gl_Position = p * vec4(actualPosition, 0.0f, 1.0f);\n"
"colour = inColour;\n"
"}\0";

const char fShader[] =
"#version 440 core\n"
"in vec3 colour;\n"
"out vec4 fragColour;\n"
"void main()\n"
"{\n"
"fragColour = vec4(colour, 1.0f);\n"
"}\0";

void initGraphicalData(Platform* platform, GraphicalData* graphicalData, Vec2f paddleSize, Vec2f ballSize)
{
  	Drawable paddleDrawable = loadRect(paddleSize.x, paddleSize.y, 1.0f, 1.0f, 1.0f);
  	graphicalData->drawables[PADDLE_DRAWABLE] = paddleDrawable;
	
  	Drawable ballDrawable = loadRect(ballSize.x, ballSize.y, 1.0f, 1.0f, 1.0f);
  	graphicalData->drawables[BALL_DRAWABLE] = ballDrawable;
	
	graphicalData->shader = loadShader(vShader, fShader);
	graphicalData->translationUbo = createUniformBuffer(sizeof(glm::mat4) + sizeof(glm::mat4), 0);
	assert(graphicalData->shader && graphicalData->translationUbo);
	setTranslationBlock(platform, graphicalData->translationUbo, COURT_WIDTH, COURT_HEIGHT);
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
  	useShader(graphicalData->shader);
	
	for(uint entity = PADDLE_LEFT; entity <= BALL; entity++)
	{
		Vec2f position = getEntityPosition(entities, entity);
		Vec2f size = getEntitySize(entities, entity);
		uint d = (isEntityType(entities, entity, ENTITY_PADDLE)) ? PADDLE_DRAWABLE : BALL_DRAWABLE;
		Drawable drawable = graphicalData->drawables[d];
		drawEntity(graphicalData->shader, drawable, position, size);
	}
}
