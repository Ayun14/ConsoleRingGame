#include<Windows.h>
#include <random>
#include<string>
#include<Digitalv.h>
#include <mmsystem.h>
#include "Console.h"
#pragma comment(lib, "winmm.lib")

void FullScreen()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	::SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
}

void Gotoxy(int x, int y)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cur = { x, y }; // {_x*2,_y}가 더 자연스러울 수 있음.
	SetConsoleCursorPosition(out, cur);
}

BOOL GotoPos(int x, int y)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cur = { x, y }; // {_x*2,_y}가 더 자연스러울 수 있음.
	return SetConsoleCursorPosition(out, cur);
}

COORD CursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buf);
	return buf.dwCursorPosition;
}

void CursorVis(bool vis, DWORD size)
{
	CONSOLE_CURSOR_INFO curinfo;
	curinfo.dwSize = size; // 커서굵기 1~100
	curinfo.bVisible = vis; // on, off
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curinfo);
}

void SetColor(int textColor, int bgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE)
		, (bgColor << 4) | textColor);
}

int GetColor()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	int color = info.wAttributes & 0xf;
	return color;
}

void LockResize()
{
	HWND console = GetConsoleWindow();
	if (nullptr != console) // 요다 표현법
	{
		LONG style = GetWindowLong(console, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;// &~WS_CAPTION;
		SetWindowLong(console, GWL_STYLE, style);
	}
}

COORD GetConsoleResolution()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE)
		, &info);
	short width = info.srWindow.Right - info.srWindow.Left + 1;
	short height = info.srWindow.Bottom - info.srWindow.Top + 1;
	return COORD{ width, height };
}

void SetFontSize(UINT weight, UINT fontX, UINT fontY)
{
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

	GetCurrentConsoleFontEx(hout, false, &font);
	font.FontWeight = weight;
	font.dwFontSize.X = fontX;
	font.dwFontSize.Y = fontY;
	SetCurrentConsoleFontEx(hout, false, &font);
}

UINT Bgmid;
UINT Effectid;
void PlayBgm(LPCWSTR _soundname, int _volume)
{
	mciSendCommand(Bgmid, MCI_CLOSE, NULL, (DWORD)NULL); // RAII(중지)

	MCI_OPEN_PARMS openBgm;
	// mp3: mpegvideo, wav: waveaudio, avi: avivideo
	openBgm.lpstrDeviceType = TEXT("mpegvideo");
	openBgm.lpstrElementName = _soundname;
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openBgm);
	Bgmid = openBgm.wDeviceID;

	std::wstring msg = L"setaudio " + std::wstring(_soundname) +
		L" volume to " + std::to_wstring(_volume);
	mciSendString(msg.c_str(), NULL, NULL, NULL);

	// PLAY
	MCI_PLAY_PARMS playBgm;
	mciSendCommand(Bgmid, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&playBgm);

}

void PlayEffect(LPCWSTR _soundname, int _volume)
{
	MCI_OPEN_PARMS openEffect;
	// mp3: mpegvideo, wav: waveaudio, avi: avivideo
	openEffect.lpstrDeviceType = TEXT("waveaudio");
	openEffect.lpstrElementName = _soundname;
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openEffect);
	Effectid = openEffect.wDeviceID;

	std::wstring volumeCmd = L"setaudio " + std::wstring(_soundname) +
		L" volume to " + std::to_wstring(_volume);
	mciSendString(volumeCmd.c_str(), NULL, NULL, NULL);
	
	mciSendCommand(Effectid, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)NULL);

	MCI_PLAY_PARMS playEffect;
	mciSendCommand(Effectid, MCI_PLAY, MCI_WAIT, (DWORD_PTR)&playEffect);
	
	mciSendCommand(Effectid, MCI_CLOSE, NULL, (DWORD_PTR)NULL);
}


