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

void Init(PPLAYER player, vector<LINES>& linesVec)
{
	system("title RingRunner | mode con cols=80 lines=40");
	CursorVis(false, 1);
	LockResize();

	srand((unsigned int)time(NULL));

	moveTime = 35;
	lastSpawnLineLength = 0;

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

	player->isEasyLine = false;
	player->isHardLine = false;

	LINES initialLine = { { 0, 17 }, MAP_WIDTH };
	linesVec.push_back(initialLine);
}

void Update(PPLAYER player, vector<LINES>& linesVec, vector<ITEM>& itemVec, int* score)
{
	// Plyer
	MoveUpdate(player);

	// Line
	LineUpdate(linesVec);

	// Item
	ItemUpdate(linesVec, itemVec);
	ItemCollisionCheck(player, itemVec, score);

	if (player->isEasyLine)
	{
		clock_t lastTime = clock();

		int orginMoveTime = moveTime;
		int slowMoveTime = 55;
		moveTime = slowMoveTime;

		if (clock() - lastTime >= 5000)
		{
			moveTime = orginMoveTime;
			player->isEasyLine = false;
		}
	}
	else if (player->isHardLine)
	{
		clock_t lastTime = clock();

		int orginMoveTime = moveTime;
		int fastMoveTime = 15;
		moveTime = fastMoveTime;

		if (clock() - lastTime >= 5000)
		{
			moveTime = orginMoveTime;
			player->isHardLine = false;
		}
	}

	// Score
	static clock_t lastTime = clock();
	if (clock() - lastTime >= CLOCKS_PER_SEC)
	{
		++*score;
		lastTime = clock();
	}
}

void MoveUpdate(PPLAYER player)
{
	player->newPos = player->pos;

	if (GetAsyncKeyState(VK_UP) & 0x8000 && player->pos.y > 0)
		--player->newPos.y;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && player->pos.y < MAP_HEIGHT - 8)
		++player->newPos.y;

	player->pos = player->newPos;
}

void LineUpdate(vector<LINES>& linesVec)
{
	static clock_t lastSpawnTime = clock();
	if (clock() - lastSpawnTime >= moveTime)
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
			newLinesLength = rand() % 6 + 5; // 5 ~ 10
		}
		else
		{
			int lastLineY = linesVec.back().pos.y;
			newLineY = lastLineY + (rand() % 3 - 1); // -1, 0, 1

			newLinesLength = rand() % 6 + 5; // 5 ~ 10

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

			if ((lineX >= player->pos.x + 6 && lineX <= player->pos.x + 13) &&
				(lineY == player->pos.y || lineY == player->pos.y + 7))
			{
				Gotoxy(lineX, lineY);
				SetColor((int)COLOR::LIGHT_RED);
				cout << "▒";
				SetColor((int)COLOR::WHITE);
				Sleep(1200);
				return true;
			}
		}
	}

	return false;
}

void ItemUpdate(vector<LINES>& linesVec, vector<ITEM>& itemVec)
{
	// item move
	static clock_t lastMoveTime = clock();
	if (clock() - lastMoveTime >= moveTime)
	{
		for (auto& item : itemVec)
			item.pos.x--;

		if (!itemVec.empty())
			if (itemVec.front().pos.x < 0) itemVec.erase(itemVec.begin());

		lastMoveTime = clock();
	}

	// item spawn
	static clock_t lastSpawnTime = clock();
	if (clock() - lastSpawnTime >= 7000)
	{
		lastSpawnTime = clock();

		int spawnPersent = rand() % 10;
		if (spawnPersent < 9)
		{
			spawnPersent = rand() % 4;
			ITEMTYPE spawnItemType = ITEMTYPE::NONE;
			if (spawnPersent == 0)
				spawnItemType = ITEMTYPE::EASYLINE;
			else if (spawnPersent == 1)
				spawnItemType = ITEMTYPE::HARDLINE;
			else if (spawnPersent == 2)
				spawnItemType = ITEMTYPE::SCOREUP;
			else
				spawnItemType = ITEMTYPE::SCOREDOWN;

			int randY = 0;
			while (true)
			{
				randY = linesVec.back().pos.y + (rand() % 9 - 4);
				if (randY != linesVec.back().pos.y) break;
			}
			 
			POS itemPos = { MAP_WIDTH - 1, randY };
			itemVec.push_back({ itemPos, spawnItemType });
		}
	}
}

void ItemCollisionCheck(PPLAYER player, vector<ITEM>& itemVec, int *score)
{
	auto it = itemVec.begin();
	while (it != itemVec.end())
	{
		ITEM& item = *it;
		if (item.pos.x < player->pos.x || item.pos.x > player->pos.x + 20)
		{
			++it;
			continue;
		}

		if (player->pos.y == item.pos.y ||
			player->pos.y + 7 == item.pos.y)
		{
			if (player->pos.x + 13 != item.pos.x)
			{
				++it;
				continue;
			}
		}
		else if (player->pos.y + 1 == item.pos.y ||
			player->pos.y + 6 == item.pos.y)
		{
			if (player->pos.x + 15 != item.pos.x)
			{
				++it;
				continue;
			}
		}
		else if (player->pos.y + 2 == item.pos.y ||
			player->pos.y + 5 == item.pos.y)
		{
			if (player->pos.x + 17 != item.pos.x)
			{
				++it;
				continue;
			}
		}
		else if (player->pos.y + 3 == item.pos.y ||
			player->pos.y + 4 == item.pos.y)
		{
			if (player->pos.x + 18 != item.pos.x)
			{
				++it;
				continue;
			}
		}
		else
		{
			++it;
			continue;
		}

		if (item.itemType == ITEMTYPE::EASYLINE)
			player->isEasyLine = true;
		else if (item.itemType == ITEMTYPE::HARDLINE)
			player->isHardLine = true;
		else if (item.itemType == ITEMTYPE::SCOREUP)
			*score += 10;
		else if (item.itemType == ITEMTYPE::SCOREDOWN)
			*score -= 10;

		it = itemVec.erase(it);
	}
}

void Render(PPLAYER player, vector<LINES>& linesVec, vector<ITEM>& itemVec, int score)
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

	// Render Item
	for (const auto& item : itemVec)
	{
		Gotoxy(item.pos.x, item.pos.y);
		if (item.itemType == ITEMTYPE::EASYLINE)
		{
			SetColor((int)COLOR::LIGHT_BLUE);
			cout << "◆";
		}
		else if (item.itemType == ITEMTYPE::HARDLINE)
		{
			SetColor((int)COLOR::LIGHT_RED);
			cout << "◈";
		}
		else if (item.itemType == ITEMTYPE::SCOREUP)
		{
			SetColor((int)COLOR::LIGHT_BLUE);
			cout << "▲";
		}
		else if (item.itemType == ITEMTYPE::SCOREDOWN)
		{
			SetColor((int)COLOR::LIGHT_RED);
			cout << "▼";
		}
	}

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
