#include <Windows.h>
#include "GameLogic.h"
#include "Title.h"
#include "Console.h"
#include "GameOver.h"

int main()
{
	srand((unsigned int)time(NULL));

	PLAYER player;
	vector<LINES> linesVec;
	vector<ITEM> itemVec;

	int score = 0;
	int bestScore = 0;
	PlayBgm(TEXT("TitleBgm.mp3"), 300);
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