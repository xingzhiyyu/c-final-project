#pragma once

#include <graphics.h>
#include <tchar.h>
#include "config.h"
#include "logic.h"


// 绘图接口
void DrawGame(const struct circle* p, int canFly);
void DrawStartScreen(int scrW, int scrH, int isLoggedIn, int, int);
// 新增：绘制方块接口
void DrawBox(int x, int y, int size, LPCTSTR text, ColorPair pair);
void drawlogininterface();
void DrawLinkList(linknode* head, int size);

void drawlosinginterface(int scrW, int scrH, int inRestartBtn, int inQuitBtn);
