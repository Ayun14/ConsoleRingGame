#pragma once
void TitleRender();
bool Title();
enum class MENU
{
	START, INFO, QUIT
};
void InfoRender();
MENU MenuRender();
enum class KEY
{
	UP, DOWN, SPACE, FAIL
};
KEY KeyController();
