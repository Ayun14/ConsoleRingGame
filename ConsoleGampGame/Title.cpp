#include<iostream>
#include<fcntl.h>
#include<io.h>
#include<Windows.h>
#include "Title.h"
#include "Console.h"
#include "GameLogic.h"
using namespace std;

void TitleRender()
{
	PlayBgm(TEXT("TitleBgm.mp3"), 300);
	int prevmode = _setmode(_fileno(stdout), _O_U16TEXT);
	SetColor((int)COLOR::LIGHT_YELLOW);
	wcout << L"   _____________                    ________" << endl;
	wcout << L"   ___  __ \\__(_)_____________ _    ___  __ \\___  ____________________________" << endl;
	wcout << L"   __  /_/ /_  /__  __ \\_  __  /    __  /_/ /  / / /_  __ \\_  __ \\  _ \\_  ___/" << endl;
	wcout << L"   _  _, _/_  / _  / / /  /_/ /     _  _, _// /_/ /_  / / /  / / /  __/  /" << endl;
	wcout << L"   /_/ |_| /_/  /_/ /_/_\\__, /      /_/ |_| \\__,_/ /_/ /_//_/ /_/\\___//_/" << endl;
	wcout << L"                       /____/" << endl;
	SetColor((int)COLOR::WHITE);
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
			EnterAnimation();
			PlayBgm(TEXT("bgm.mp3"),300);
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

void EnterAnimation()
{
	COORD Resolution = GetConsoleResolution();
	int width = Resolution.X;
	int height = Resolution.Y;
	int animtime = 20;

	SetColor((int)COLOR::BLACK, (int)COLOR::LIGHT_YELLOW);
	for (int i = 0; i < width; i+=2)
	{
		for (int j = 0; j < height; j++)
		{
			Gotoxy(i, j); //세로방향으로 텍스트 출력 
			cout << "  ";
		}

		Sleep(animtime);
	}

	Sleep(30);
	SetColor((int)COLOR::WHITE, (int)COLOR::BLACK);
	for (int i = width; i > 0; i -= 2)
	{
		for (int j = 0; j < height; j++)
		{
			Gotoxy(i, j); //세로방향으로 텍스트 출력 
			cout << "  ";
		}

		Sleep(animtime);
	}

	SetColor((int)COLOR::WHITE, (int)COLOR::BLACK);
	system("cls");
}

void InfoRender()
{
	system("cls");

	cout << "[조작법]" << endl;
	cout << " - 위로 이동: 화살표 위 (↑)" << endl;
	cout << " - 아래로 이동: 화살표 아래 (↓)" << endl;
	cout << " - 점프: 스페이스바" << endl;

	cout << "\n\n";

	cout << "[게임 소개]" << endl;
	cout << "RingRunner는 링의 위 아래에 선이 닿지 않도록 조심하며" << endl;
	cout << "최대한 오래 생존하는 게임입니다. 점수를 획득하여" << endl;
	cout << "당신만의 최고 점수를 갱신하세요!" << endl;

	cout << "\n\n";

	cout << "[아이템 소개]" << endl;
	SetColor((int)COLOR::LIGHT_BLUE);
	cout << " - 쉬운 선 (◆): 선이 느리게 움직입니다." << endl;
	cout << " - 점수 증가 (▲): 점수가 10점 증가합니다." << endl;
	SetColor((int)COLOR::LIGHT_RED);
	cout << " - 어려운 선 (◈): 선이 빠르게 움직입니다." << endl;
	cout << " - 점수 감소 (▼): 점수가 10점 감소합니다." << endl;
	SetColor((int)COLOR::WHITE);

	cout << "\n\n";

	cout << "게임 정보 화면에서 나가려면 화살표 아래 (↓)를 눌러주세요." << endl;

	while (true)
	{
		if (KeyController() == KEY::DOWN)
			break;
	}

	system("cls");
}

MENU MenuRender()
{
	COORD Resolution = GetConsoleResolution();
	int x = Resolution.X / 2.5;
	int y = Resolution.Y / 2.5;
	int originy = y;
	Gotoxy(x - 3, y);  // 메뉴의 시작점에 커서를 놓습니다.
	cout << "▶ 게임 시작";
	Gotoxy(x, y + 1);
	cout << "게임 정보";
	Gotoxy(x, y + 2);
	cout << "종료 하기";

	int currentSelection = 0; // 현재 선택된 메뉴를 추적합니다.

	while (true)
	{
		KEY key = KeyController();
		switch (key)
		{
		case KEY::UP:
		{
			if (currentSelection > 0)
			{
				PlaySound(TEXT("menuSelect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Gotoxy(x - 3, y + currentSelection);
				cout << "  ";
				currentSelection--;
				Gotoxy(x - 3, y + currentSelection);
				cout << "▶";
				Sleep(100);
			}
		}
		break;
		case KEY::DOWN:
		{
			if (currentSelection < 2)
			{
				PlaySound(TEXT("menuSelect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Gotoxy(x - 3, y + currentSelection);
				cout << "  ";
				currentSelection++;
				Gotoxy(x - 3, y + currentSelection);
				cout << "▶";
				Sleep(100);
			}
		}
		break;
		case KEY::SPACE:
			PlaySound(TEXT("menuSelect.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (currentSelection == 0)
				return MENU::START;
			else if (currentSelection == 1)
				return MENU::INFO;
			else if (currentSelection == 2)
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




