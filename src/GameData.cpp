#include "GameData.hpp"

void initGameData(GameData* data)
{
	data->isPlaying = 0;
	for(uint i = 0; i < 2; i++) data->scores[i] = 0;
}

void printScore(GameData* data)
{
	printf("Score: { %d | %d }\n", data->scores[0], data->scores[1]);
}

void scoreGoal(GameData* data, uint scorer)
{
	data->scores[scorer]++;
	printScore(data);
}

void startPlay(GameData* data)
{
	assert(!(data->isPlaying));
	data->isPlaying = 1;
}

void endPlay(GameData* data)
{
	assert(data->isPlaying);
	data->isPlaying = 0;
}
