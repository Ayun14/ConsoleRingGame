#include<Windows.h>
#include<mmsystem.h>
#include<algorithm>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include<fcntl.h>
#include<io.h>
#include "GameLogic.h"
#include "console.h"
#pragma comment(lib, "winmm.lib")

void Init(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec)
{
	system("title RingRunner | mode con cols=80 lines=40");
	CursorVis(false, 1);
	LockResize();

	srand((unsigned int)time(NULL));
	
	player->pos = { MAP_WIDTH / 4, MAP_HEIGHT / 3 };

	wchar_t tempShape[8][21] = {
		L"      ████████      ",
		L"    ████    ████    ",
		L"  ████        ████  ",
		L" ███            ███ ",
		L" ███            ███ ",
		L"  ████        ████  ",
		L"    ████    ████    ",
		L"      ████████      ",
	};

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 21; ++j) 
			player->shape[i][j] = tempShape[i][j];
	}

	LINES initialLine = { { 0, 17 }, MAP_WIDTH };
	linesVec.push_back(initialLine);
}

void Update(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec, int* score)
{
	// Plyer
	MoveUpdate(arrMap, player);

	// Line
	LineUpdate(arrMap, linesVec);

	// Score
	static clock_t lastTime = clock();
	if (clock() - lastTime >= CLOCKS_PER_SEC)
	{
		++*score;
		lastTime = clock();
	}
}

void MoveUpdate(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player)
{	
	// Player 움직임 구현
	player->newPos = player->pos;
	if (GetAsyncKeyState(VK_UP) & 0x8000 && player->pos.y > 0)
		--player->newPos.y;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && player->pos.y < MAP_HEIGHT - 8)
		++player->newPos.y;

	player->pos = player->newPos;
}

void LineUpdate(char arrMap[MAP_HEIGHT][MAP_WIDTH], vector<LINES>& linesVec)
{
	static int lastSpawnLineLength = 0;

	static clock_t lastSpawnTime = clock();
	if (clock() - lastSpawnTime >= 60)
	{
		lastSpawnTime = clock();

		for (auto& lines : linesVec)
			lines.pos.x--;

		if (!linesVec.empty())
		{
			if (linesVec.front().pos.x + linesVec.front().length <= 0) // 맵 밖으로 나갔다
				linesVec.erase(linesVec.begin());
		}

		lastSpawnLineLength--;
		if (lastSpawnLineLength > 0) return;

		int newLineY;
		int newLinesLength;
		LINES newLines; // vector에 넣을 Line들을 가지고 있는 Lines
		if (linesVec.empty())
		{
			newLineY = rand() % (MAP_HEIGHT - 4) + 2;
			newLinesLength = rand() % 6 + 6; // 6 ~ 11
		}
		else
		{
			int lastLineY = linesVec.back().pos.y;
			newLineY = lastLineY + (rand() % 3 - 1); // -1, 0, 1

			newLinesLength = rand() % 6 + 6; // 6 ~ 11

			if (newLineY <= 0) newLineY = 1;
			if (newLineY >= MAP_HEIGHT) newLineY = MAP_HEIGHT - 2;
		}

		newLines.pos = { MAP_WIDTH - 2, newLineY };
		newLines.length = newLinesLength;
		linesVec.push_back(newLines);

		lastSpawnLineLength = linesVec.back().length;
	}
}

bool LineCollisionCheck(PPLAYER player, vector<LINES>& linesVec)
{
	for (const auto& lines : linesVec)
	{
		if (lines.pos.x + lines.length < player->pos.x || lines.pos.x > player->pos.x + 20)
			continue;

		for (int i = 0; i < lines.length; ++i)
		{
			int lineX = lines.pos.x + i;
			int lineY = lines.pos.y;

			if ((lineX >= player->pos.x && lineX <= player->pos.x + 20) &&
				(lineY == player->pos.y || lineY == player->pos.y + 7))
			{
				Sleep(1000);

				return true;
			}
		}
	}

	return false;
}

void Render(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec, int score)
{
	system("cls");

	// Render Score
	Gotoxy(0, 0);
	cout << "Score : " << score;

	int prevmode = _setmode(_fileno(stdout), _O_U16TEXT);

	// Render Player
	SetColor((int)COLOR::GRAY);
	for (int i = 0; i < 8; ++i)
	{
		Gotoxy(player->pos.x, player->pos.y + i);
		wcout << player->shape[i];
	}

	// Render Line
	SetColor((int)COLOR::LIGHT_YELLOW);
	for (int i = 0; i < linesVec.size(); ++i)
	{
		for (int j = 0; j < linesVec[i].length; ++j)
		{
			// 맵 밖으로 나간거면 안 그리기
			if (linesVec[i].pos.x + j >= MAP_WIDTH) continue;
			if (linesVec[i].pos.x + j <= 0) continue;

			// 링의 오른쪽에 겹쳐서 그려지는 중이면 안 그리기
			if (player->pos.y + 1 == linesVec[i].pos.y ||
				player->pos.y + 6 == linesVec[i].pos.y)
			{
				if (player->pos.x + 12 <= linesVec[i].pos.x + j &&
					player->pos.x + 15 >= linesVec[i].pos.x + j)
					continue;
			}
			else if (player->pos.y + 2 == linesVec[i].pos.y ||
				player->pos.y + 5 == linesVec[i].pos.y)
			{
				if (player->pos.x + 14 <= linesVec[i].pos.x + j &&
					player->pos.x + 17 >= linesVec[i].pos.x + j)
					continue;
			}
			else if (player->pos.y + 3 == linesVec[i].pos.y||
				player->pos.y + 4 == linesVec[i].pos.y)
			{
				if (player->pos.x + 16 <= linesVec[i].pos.x + j &&
					player->pos.x + 18 >= linesVec[i].pos.x + j)
					continue;
			}

			// 위에 안걸리면 그려주기
			Gotoxy(linesVec[i].pos.x + j, linesVec[i].pos.y);
			wcout << L"█";
		}
	}
	int curmode = _setmode(_fileno(stdout), prevmode);

	SetColor((int)COLOR::WHITE);
}


void FrameSync(unsigned int framerate)
{
	static DWORD frameTime = 1000 / framerate;

	clock_t oldtime = clock();
	clock_t currentTime;

	while (true)
	{
		currentTime = clock();
		if (currentTime - oldtime > frameTime)
		{
			oldtime = currentTime;
			break;
		}
	}
}
