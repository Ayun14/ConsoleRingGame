#include<iostream>
#include<fcntl.h>
#include<io.h>
#include<Windows.h>
#include "GameOver.h"
#include "Title.h"
#include "Console.h"
using namespace std;


void GameOverRender()
{
	system("cls");
	PlayBgm(TEXT("GameOverBgm.mp3"), 300);

	int prevmode = _setmode(_fileno(stdout), _O_U16TEXT);
	wcout << L"    ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗\n";
	wcout << L"   ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗\n";
	wcout << L"   ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝\n";
	wcout << L"   ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n";
	wcout << L"   ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n";
	wcout << L"    ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\n";
	int curmode = _setmode(_fileno(stdout), prevmode);
}

bool Retry(int bestScore, int score)
{
	while (true)
	{
		COORD Resolution = GetConsoleResolution();
		GameOverRender();
		int y = Resolution.Y / 5;
		Gotoxy(0, y);
		cout << "Best Score: " << bestScore << endl;
		cout << "Score: " << score << endl;
		cout << "\n\n";

		MENU menu = MenuRender();
		switch (menu)
		{
		case MENU::START:
			EnterAnimation();
			return true;
			break;
		case MENU::INFO:
			InfoRender();
			break;
		case MENU::QUIT:
			return false;
			break;
		}
	}
}
