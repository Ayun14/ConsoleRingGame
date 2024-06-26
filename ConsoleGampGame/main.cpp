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
	PLAYER player;
	vector<LINES> linesVec;
	vector<ITEM> itemVec;

	int score = 0;
	int bestScore = 0;

	Init(&player, linesVec);

	if (!Title())
		return 0;
	else
	{
		while (true)
		{
			Init(&player, linesVec);

			while (!LineCollisionCheck(&player, linesVec))
			{
				Update(&player, linesVec, itemVec, &score);
				Gotoxy(0, 0);
				Render(&player, linesVec, itemVec, score);
				FrameSync(60);
			}

			if (score > bestScore)
				bestScore = score;

			if (!Retry(bestScore, score))
				return 0;

			score = 0;
			linesVec.clear();
			itemVec.clear();
		}
	}
}