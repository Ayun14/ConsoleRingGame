#pragma once
void TitleRender();
bool Title();
enum class MENU
{
	START, QUIT
};
MENU MenuRender();
enum class KEY
{
	UP, DOWN, SPACE, FAIL
};
KEY KeyController();
