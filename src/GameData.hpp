#pragma once
#include <cstdio>
#include <assert.h>
#include "Types.hpp"


struct GameData
{
	byte isPlaying;
	byte scores[2];
};

void initGameData(GameData*);
void printScore(GameData*);
void scoreGoal(GameData*, uint scorer);
void startPlay(GameData*);
void endPlay(GameData*);
