#include <Windows.h>
#include<fcntl.h>
#include<io.h>
#include "GameLogic.h"
#include "Title.h"
#include "console.h"
#include "GameOver.h"
#include <iostream>

int main()
{
	char arrMap[MAP_HEIGHT][MAP_WIDTH] = {};
	PLAYER player;
	vector<LINES> linesVec;

	int score = 0;
	int bestScore = 0;

	Init(arrMap, &player, linesVec);

	if (!Title())
		return 0;
	else
	{
		while (true)
		{
			Init(arrMap, &player, linesVec);

			while (!LineCollisionCheck(&player, linesVec))
			{
				Update(arrMap, &player, linesVec, &score);
				Gotoxy(0, 0);
				Render(arrMap, &player, linesVec, score);
				FrameSync(60);
			}

			if (score > bestScore)
				bestScore = score;

			if (!Retry(bestScore, score))
				return 0;

			score = 0;
			linesVec.clear();
		}
	}
}