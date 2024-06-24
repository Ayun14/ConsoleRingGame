#pragma once
const int MAP_WIDTH = 80;
const int MAP_HEIGHT = 40;

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
	bool isInvincibility; // 무적
	// 디버프 아이템
	bool isHardLine; // 선 난이도 높아짐
	bool isReversal; // 입력키 반전
	
}PLAYER, * PPLAYER;

typedef struct _taglines
{
	POS pos; // 맨앞 위치
	int length; // 선 길이
} LINES, *PLINES;

#include<vector>
using std::vector;
void Init(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec);
void Update(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec, int* score);
void MoveUpdate(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player);
void LineUpdate(char arrMap[MAP_HEIGHT][MAP_WIDTH], vector<LINES>& linesVec);
bool LineCollisionCheck(PPLAYER player, vector<LINES>& linesVec);
void Render(char arrMap[MAP_HEIGHT][MAP_WIDTH], PPLAYER player, vector<LINES>& linesVec, int score);
void FrameSync(unsigned int framerate);
