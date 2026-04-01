#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>
#include "draw.h"
#include "logic.h"

//display is 960 * 640，middle(480*320)

static const ColorPair colorTable[] = {
    {RGB(238, 228, 218), RGB(119, 110, 101)}, // 2
    {RGB(237, 224, 200), RGB(119, 110, 101)}, // 4
    {RGB(242, 177, 121), RGB(255, 255, 255)}, // 8
    {RGB(245, 149, 99),  RGB(255, 255, 255)}, // 16
    {RGB(246, 124, 95),  RGB(255, 255, 255)}, // 32
    {RGB(246, 94, 59),   RGB(255, 255, 255)}, // 64
    {RGB(237, 207, 114), RGB(255, 255, 255)}, // 128

};

// 辅助函数：绘制单个方块
void DrawBox(int x, int y, int size, LPCTSTR text, ColorPair pair) {
    // 1. 绘制方块底色和边框
    setfillcolor(pair.bg);
    setlinecolor(BLACK);
    fillrectangle(x, y, x + size, y + size);
    rectangle(x, y, x + size, y + size);

    // 2. 设置字体样式
    settextcolor(pair.text);
    setbkmode(TRANSPARENT);
    settextstyle(30, 0, _T("Consolas"));

    // 3. 计算文字居中 
    int w = textwidth(text);
    int h = textheight(text);

    // 4. 绘制文字
    outtextxy(x + ((size - w) / 2), y + ((size - h) / 2), text);
}
void DrawLinkList(linknode* head, int size) {
    if (head == NULL) return;

    linknode* p = head->next;

    while (p != NULL) {

        // 每个节点有 8 个点
        for (int i = 0; i < 8; i++) {

            int x = (int)p->Pos[i].x;
            int y = (int)p->Pos[i].y;
            ColorPair pair = colorTable[2];
            DrawBox(x, y, size, _T("2"), pair);
        }

        p = p->next;
    }
}


void DrawStartScreen(int scrW, int scrH, int isLoggedIn, int inloginbutton, int instartbutton) {
    cleardevice();

    int btnW = 220;
    int btnH = 60;
    int gap = 24;
    int left = scrW / 2 - btnW / 2;
    int startTop = scrH / 2 - 40;
    int loginTop = startTop + btnH + gap;

    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(48, 0, _T("Consolas"));
    outtextxy(scrW / 2 - textwidth(_T("Flappy 2048")) / 2, scrH / 2 - 150, _T("Flappy 2048"));

    setlinecolor(WHITE);

    if (instartbutton == 0) {
        setfillcolor(RGB(60, 140, 230));
    }
    else if (instartbutton == 1) {
        setfillcolor(RGB(20, 100, 190));
    }
    fillroundrect(left, startTop, left + btnW, startTop + btnH, 12, 12);
    settextstyle(28, 0, _T("Consolas"));
    outtextxy(left + (btnW - textwidth(_T("START"))) / 2, startTop + (btnH - textheight(_T("START"))) / 2, _T("START"));

    setfillcolor(inloginbutton ? RGB(40, 90, 60) : RGB(90, 160, 110));
    fillroundrect(left, loginTop, left + btnW, loginTop + btnH, 12, 12);
    outtextxy(left + (btnW - textwidth(isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"))) / 2, loginTop + (btnH - textheight(isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"))) / 2,
        isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"));
}

void DrawGame(const struct circle* p, int canFly) {
    cleardevice();

    // --- 绘制能量条 ---
    double energyRatio = p->energy / p->maxenergy;
    setlinecolor(WHITE);
    rectangle(20, 20, 220, 40);
    setfillcolor(!canFly ? DARKGRAY : (energyRatio > 0.3 ? CYAN : RED));
    fillrectangle(22, 22, (int)(22 + 196 * energyRatio), 38);

    // --- 绘制 2048 小方块 ---
    // 颜色配置: 2048风格 (2的背景色)
    int data = p->data;
    ColorPair color = colorTable[(int)(log(data) / log(2)) % 7];

    // 计算方块左上角坐标 (原始 p->Pos 是中心点)
    int boxSize = 80;
    int x = (int)p->Pos.x - boxSize / 2;
    int y = (int)p->Pos.y - boxSize / 2;

    // 转换方块的数据为字符串
    TCHAR Text[15];
    _stprintf_s(Text, 15, _T("%d"), data);
    DrawBox(x, y, boxSize, Text, color);
}
void drawlogininterface() {
    cleardevice();
    settextcolor(WHITE);
    settextstyle(36, 0, _T("Consolas"));
    outtextxy(480 - 0.5 * textwidth(_T("please enter your username")), 160 - 0.5 * textheight(_T("please enter your username")), _T("please enter your username"));
    fillrectangle(200, 200, 760, 240);
}