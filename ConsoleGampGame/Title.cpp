#include<iostream>
#include<fcntl.h>
#include<io.h>
#include<Windows.h>
#include "Title.h"
#include "console.h"
using namespace std;

void TitleRender()
{
	int prevmode = _setmode(_fileno(stdout), _O_U16TEXT);
	wcout << L"   _____________                    ________" << endl;
	wcout << L"   ___  __ \__(_)_____________ _    ___  __ \___  ____________________________" << endl;
	wcout << L"   __  /_/ /_  /__  __ \_  __ `/    __  /_/ /  / / /_  __ \_  __ \  _ \_  ___/" << endl;
	wcout << L"   _  _, _/_  / _  / / /  /_/ /     _  _, _// /_/ /_  / / /  / / /  __/  /" << endl;
	wcout << L"   /_/ |_| /_/  /_/ /_/_\__, /      /_/ |_| \__,_/ /_/ /_//_/ /_/\___//_/" << endl;
	wcout << L"                       /____/" << endl;
	int curmode = _setmode(_fileno(stdout), prevmode);
}

bool Title()
{
	while (true)
	{
		COORD Resolution = GetConsoleResolution();
		int y = Resolution.Y / 5;
		Gotoxy(0, y);
		TitleRender();
		MENU menu = MenuRender();
		switch (menu)
		{
		case MENU::START:
			// 애니메이션 등등..
			return true;
			break;
		case MENU::QUIT:
			return false;
			break;
		}
	}
}

MENU MenuRender()
{
	COORD Resolution = GetConsoleResolution();
	int x = Resolution.X / 2.5;
	int y = Resolution.Y / 2.5;
	int originy = y;
	Gotoxy(x, y);
	cout << "게임 시작";
	Gotoxy(x, y + 1);
	cout << "종료 하기";
	while (true)
	{
		KEY key = KeyController();
		switch (key)
		{
		case KEY::UP:
		{
			if (y > originy)
			{
				Gotoxy(x - 2, y);
				cout << " ";
				Gotoxy(x - 2, --y);
				cout << ">";
				Sleep(100);
			}
		}
		break;
		case KEY::DOWN:
		{
			if (y < originy + 1)
			{
				Gotoxy(x - 2, y);
				cout << " ";
				Gotoxy(x - 2, ++y);
				cout << ">";
				Sleep(100);
			}
		}
		break;
		case KEY::SPACE:
			if (originy == y)
				return MENU::START;
			else if (originy + 1 == y)
				return MENU::QUIT;
		}
	}
}

KEY KeyController()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		return KEY::UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		return KEY::DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		return KEY::SPACE;

	return KEY::FAIL;
}
