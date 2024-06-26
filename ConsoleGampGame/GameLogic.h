#pragma once
const int MAP_WIDTH = 80;
const int MAP_HEIGHT = 40;

static int moveTime; // 선, 아이템 움직이는 시간(스피드)
static int lastSpawnLineLength = 0; // 마지막으로 생성된 선의 길이

typedef struct _tagpos
{
	int x;
	int y;
	bool operator ==(const _tagpos& _pos)
	{
		return x == _pos.x && y == _pos.y;
	}

}POS, * PPOS;

typedef struct _tagplayer
{
	POS pos;
	POS newPos;
	int playerSizes[4] = { 8, 12, 16, 17};
	wchar_t shape[8][21];

	// 버프 아이템
	bool isEasyLine; // 선 난이도 낮아짐
	// 디버프 아이템
	bool isHardLine; // 선 난이도 높아짐

}PLAYER, * PPLAYER;

typedef struct _taglines
{
	POS pos; // 맨앞 위치
	int length; // 선 길이

} LINES, *PLINES;

enum class ITEMTYPE
{
	EASYLINE, HARDLINE, SCOREUP, SCOREDOWN, NONE
};

typedef struct _tagitem
{
	POS pos;
	ITEMTYPE itemType; // 선 길이

} ITEM, *PITEM;

#include<vector>
using std::vector;
void Init(PPLAYER player, vector<LINES>& linesVec);
void Update(PPLAYER player, vector<LINES>& linesVec, vector<ITEM>& itemVec, int* score);
void MoveUpdate(PPLAYER player);
void LineUpdate(vector<LINES>& linesVec);
bool LineCollisionCheck(PPLAYER player, vector<LINES>& linesVec);
void ItemUpdate(vector<LINES>& linesVec, vector<ITEM>& itemVec);
void ItemCollisionCheck(PPLAYER player, vector<ITEM>& itemVec, int *score);
void Render(PPLAYER player, vector<LINES>& linesVec, vector<ITEM>& itemVec, int score);
void FrameSync(unsigned int framerate);
