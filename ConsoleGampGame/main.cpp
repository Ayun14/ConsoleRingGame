#include <Windows.h>
#include<fcntl.h>
#include<io.h>
#include "GameLogic.h"
#include "Title.h"
#include "console.h"
#include <iostream>

int main()
{
	char arrMap[MAP_HEIGHT][MAP_WIDTH] = {};
	PLAYER player;
    vector<LINES> linesVec;

	Init(arrMap, &player, linesVec);

	if (!Title())
		return 0;
	else
	{
		while (!isGameOver)
		{
			Update(arrMap, &player, linesVec);
			Gotoxy(0, 0);
			Render(arrMap, &player, linesVec);
			FrameSync(60);
		}
	}

	system("cls");
	Gotoxy(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	cout << "Game Over! Final Score: " << score << endl;
}