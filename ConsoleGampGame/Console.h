#pragma once
void FullScreen();
void Gotoxy(int x, int y);
BOOL GotoPos(int x, int y);
COORD CursorPos();
void CursorVis(bool vis, DWORD size);
void SetColor(int textColor = 15, int bgColor = 0);
int GetColor();
void LockResize();
COORD GetConsoleResolution();
void SetFontSize(UINT weight, UINT fontX, UINT fontY);
enum class COLOR
{
	BLACK, BLUE, GREEN, SKYBLUE, RED,
	VOILET, YELLOW, LIGHT_GRAY, GRAY, LIGHT_BLUE,
	LIGHT_GREEN, MINT, LIGHT_RED, LIGHT_VIOLET,
	LIGHT_YELLOW, WHITE, END
};
#include<iostream>
using std::cout;
using std::wcout;
using std::endl;