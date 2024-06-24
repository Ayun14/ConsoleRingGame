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
	int score = 0;

	Init(arrMap, &player, linesVec);

	if (!Title())
		return 0;
	else
	{
		while (!LineCollisionCheck(&player, linesVec))
		{
			Update(arrMap, &player, linesVec, &score);
			Gotoxy(0, 0);
			Render(arrMap, &player, linesVec, score);
			FrameSync(60);
		}
	}

	system("cls");
	cout << "\n\n";

	int prevmode = _setmode(_fileno(stdout), _O_U16TEXT);
	wcout << L"    ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗\n";
	wcout << L"   ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗\n";
	wcout << L"   ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝\n";
	wcout << L"   ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n";
	wcout << L"   ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n";
	wcout << L"    ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\n";
	int curmode = _setmode(_fileno(stdout), prevmode);

	Gotoxy(MAP_WIDTH / 3, MAP_HEIGHT / 4);
	cout << "Score: " << score << endl;
}