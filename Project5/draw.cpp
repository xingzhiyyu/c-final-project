#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>
#include "draw.h"
#include "logic.h"
#include "config.h"

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
    int origin_width = textwidth(text);
    int final_width = 80;
    int font_size = 30;
    if (origin_width > final_width) {
		font_size = (int)(font_size * (final_width / (double)origin_width));
    }
    // 2. 设置字体样式
    settextcolor(pair.text);
    setbkmode(TRANSPARENT);
    settextstyle(font_size, 0, _T("Consolas"));

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
            long long val = p->data[i];  // 或者 p->data[0]，根据你需要
            double d = (double)val;      // 转成 double
            ColorPair pair = colorTable[(int)(log(d) / log(2))%7];
            wchar_t buf[20];
            swprintf(buf, 20, L"%d", p->data[i]);  // 或 _itow(num, buf, 10)
            LPCTSTR str = buf;
            DrawBox(x, y, size, buf, pair);
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
    if (isLoggedIn == 1)setfillcolor(RGB(40, 90, 60));
    fillroundrect(left, loginTop, left + btnW, loginTop + btnH, 12, 12);
    outtextxy(left + (btnW - textwidth(isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"))) / 2, loginTop + (btnH - textheight(isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"))) / 2,
        isLoggedIn ? _T("LOGGED IN") : _T("LOGIN"));
}

void DrawGame(const struct circle* p, int canFly) {
    

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
	outtextxy(480 - 0.5 * textwidth(_T("Trying to fix this part")), 280 - 0.5 * textheight(_T("Try to fix this part")), _T("Trying to fix this part"));
    
}
void drawlosinginterface(int scrW, int scrH, int inRestartBtn, int inQuitBtn,int inSaveBtn) {
  
    // Game Over 标题
    setbkmode(TRANSPARENT);
    settextcolor(RED);
    settextstyle(48, 0, _T("Consolas"));
    outtextxy(scrW / 2 - textwidth(_T("Game Over!")) / 2, 180, _T("Game Over!"));

    // 按钮参数
    int btnW = 220;
    int btnH = 60;
    int gap = 24;
    int left = scrW / 2 - btnW / 2;
    int restartTop = scrH / 2 - 80;
    int quitTop = restartTop + btnH + gap;
    int saveTop = quitTop + btnH + gap + 60;

    setlinecolor(WHITE);

    // Restart 按钮
    setfillcolor(inRestartBtn ? RGB(20, 100, 190) : RGB(60, 140, 230));
    fillroundrect(left, restartTop, left + btnW, restartTop + btnH, 12, 12);
    settextcolor(WHITE);
    settextstyle(28, 0, _T("Consolas"));
    outtextxy(left + (btnW - textwidth(_T("RESTART"))) / 2, restartTop + (btnH - textheight(_T("RESTART"))) / 2, _T("RESTART"));

    // Quit 按钮
    setfillcolor(inQuitBtn ? RGB(140, 40, 40) : RGB(200, 60, 60));
    fillroundrect(left, quitTop, left + btnW, quitTop + btnH, 12, 12);
    outtextxy(left + (btnW - textwidth(_T("QUIT"))) / 2, quitTop + (btnH - textheight(_T("QUIT"))) / 2, _T("QUIT"));

    outtextxy(scrW/2 - textwidth(_T("Save Your Data?"))/2, saveTop - 50, _T("Save Your Data?"));
    //save按钮
    setfillcolor(inSaveBtn ? RGB(40, 160, 40) : RGB(60,180,60));
    fillroundrect(left, saveTop, left + btnW, saveTop + btnH, 12, 12);
    outtextxy(left + (btnW - textwidth(_T("SAVE"))) / 2, saveTop + (btnH - textheight(_T("SAVE"))) / 2, _T("SAVE"));
       
}
void RenderToast(UI_Toast* toast) {
    if (!toast || !toast->active) return;

    // 1. 游戏背景色 (需对应你 setbkcolor 的值)
    int bgR = 131, bgG = 181, bgB = 217;

    // 2. 计算当前比例 (1.0 -> 0.0)
    float ratio = (float)toast->timer / toast->duration;

    // 3. 计算框的颜色：从纯白 (255) 渐变到背景蓝 (bg)
    int boxR = bgR + (int)((255 - bgR) * ratio);
    int boxG = bgG + (int)((255 - bgG) * ratio);
    int boxB = bgB + (int)((255 - bgB) * ratio);

    // 4. 计算文字颜色：从纯黑 (0) 渐变到背景蓝 (bg)
    int txtR = bgR + (int)((0 - bgR) * ratio);
    int txtG = bgG + (int)((0 - bgG) * ratio);
    int txtB = bgB + (int)((0 - bgB) * ratio);

    // --- 绘图部分 ---
    int tw = textwidth(toast->text);
    int th = textheight(toast->text);
    int boxW = tw + 40, boxH = th + 20;
    int bx = toast->x - boxW / 2, by = toast->y - boxH / 2;

    // 画框
    setfillcolor(RGB(boxR, boxG, boxB));
    setlinecolor(RGB(boxR - 20, boxG - 20, boxB - 20)); // 边框也随之变淡
    fillroundrect(bx, by, bx + boxW, by + boxH, 15, 15);

    // 画字
    settextcolor(RGB(txtR, txtG, txtB));
    settextstyle(22, 0, _T("Microsoft YaHei"));
    outtextxy(toast->x - tw / 2, toast->y - th / 2, toast->text);

    // 逻辑更新
    toast->y -= 1;
    toast->timer--;
    if (toast->timer <= 0) toast->active = 0;
    setfillcolor(BLACK);
}