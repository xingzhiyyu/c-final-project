
#include <graphics.h>
#include "config.h"
#include "physics.h"
#include "draw.h"
#include "config.h"
#include "logic.h"
#include "files.h"
#define  _CRT_SECURE_NO_WARNINGS 

int main() {
    int count = 0;
    struct GlobalConfig cfg;
    InitConfig(&cfg);
    int order = 0;
    int isLoggedIn = 0;
    UI_Toast myToast = { 0 };
    struct circle player1;
    player1.Pos.x = 40;
    player1.Pos.y = 320;
    player1.vy = 0;
    player1.maxenergy = cfg.energy_max;
    player1.energy = cfg.energy_max;
    player1.data = 1;
    player1.isPassing = 0;
    //角色状态
    int canFly = 1;
    int isSpacePressed = 0;
	int isEnterPressed = 0;
    //UI状态
    enum { UIStart = 0, UIGame, UILosing, UIEsc, UILogging,UIFinish } uiState = UIStart;
    //鼠标位置
    int mx = 0;
    int my = 0;
    TCHAR Username[14] = _T("not_given");
    int ipb = 0;

    int isClicked = 0;
    //user状态
	int bestScore = 0;

    initgraph(cfg.scr_w, cfg.scr_h);
    BeginBatchDraw();

    ExMessage msg;

    linknode* head = (linknode*)malloc(sizeof(linknode));
    create_linklist(head);

    while (1) {
        setbkcolor(RGB(131, 181, 217));
        BeginBatchDraw();
        cleardevice();
		isClicked = 0;

        while (peekmessage(&msg)) {//获取输入


            if (msg.message == WM_KEYDOWN) {
                if (msg.vkcode == VK_ESCAPE) {
                    if (uiState == UIStart) uiState = UIEsc;
                    else if (uiState == UIGame) uiState = UIStart;
                    else if (uiState == UILogging) uiState = UIStart;
                }

                if (msg.vkcode == VK_SPACE) {
                    isSpacePressed = 1;
                }
                if (msg.vkcode == VK_RETURN) {
                    isEnterPressed = 1;
                }
            }

            else if (msg.message == WM_KEYUP) {
                if (msg.vkcode == VK_SPACE) {
                    isSpacePressed = 0;
                }
                else if (msg.vkcode == VK_RETURN) {
                    isEnterPressed = 0;
                }
            }
            else if (msg.message == WM_MOUSEMOVE) {
                mx = msg.x;
                my = msg.y;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                mx = msg.x;
                my = msg.y;
               
            }
            else if (msg.message == WM_LBUTTONUP) {
                mx = msg.x;
                my = msg.y;
                isClicked = 1;
            }
        }
		if (uiState == 0) {// 开始界面
            ResetGame(&player1, &cfg, &canFly, &order, &count, head, &isSpacePressed, (int*)&uiState);
        
            int btnWid = 220;
            int btnHei = 60;
            int gap = 24;
            int left = cfg.scr_w / 2 - btnWid / 2;
            int startTop = cfg.scr_h / 2 - 40;
            int loginTop = startTop + btnHei + gap;
            int instartbutton = 0;
            int inloginbutton = 0;

            if (mx >= left && mx <= left + btnWid && my >= startTop && my <= startTop + btnHei) {
                instartbutton = 1;
            }

            if (mx >= left && mx <= left + btnWid && my >= loginTop && my <= loginTop + btnHei) {
                inloginbutton = 1;

            }

            DrawStartScreen(cfg.scr_w, cfg.scr_h, isLoggedIn, inloginbutton, instartbutton);

            if (isClicked) {
                if (instartbutton) {
                    uiState = UIGame;
                }
                else if (inloginbutton && !isLoggedIn) {
                    uiState = UILogging;
                }
            }

        }
        else if (uiState == 3) { // ESC界面
            cleardevice();
            outtextxy(cfg.scr_w / 2 - textwidth(_T("Are you sure you want to quit?")) / 2, cfg.scr_h / 2 - 100, _T("Are you sure you want to quit?"));
            outtextxy(cfg.scr_w / 2 - textwidth(_T("Press ESC again to confirm, or click anywhere to cancel.")) / 2, cfg.scr_h / 2, _T("Press ESC again to confirm, or click anywhere to cancel."));
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                clear_linklist(head);
                free(head);
                closegraph();
                return 0;
            }
            if (msg.message == WM_KEYDOWN && msg.vkcode != VK_ESCAPE) {
                uiState = UIStart;
                FlushBatchDraw();
                continue;
            }


            // 鼠标

            if (msg.message == WM_LBUTTONDOWN) {
                uiState = UIStart;
                FlushBatchDraw();
                continue;
            }
        }
        else if (uiState == 2)
        {// 失败界面
            cleardevice();
            DrawLinkList(head, 80);
            DrawGame(&player1, canFly);
            
            int btnW = 220, btnH = 60, gap = 24;
            int left = cfg.scr_w / 2 - btnW / 2;
            int restartTop = cfg.scr_h / 2 - 80;
            int quitTop = restartTop + btnH + gap;
            int saveTop = quitTop + btnH + gap + 60;
            int inRestartBtn = 0;
            int inQuitBtn = 0;
            int inSaveBtn = 0;

            if (mx >= left && mx <= left + btnW && my >= restartTop && my <= restartTop + btnH) {
                inRestartBtn = 1;
            }
            if (mx >= left && mx <= left + btnW && my >= quitTop && my <= quitTop + btnH) {
                inQuitBtn = 1;
            }
            if (mx >= left && mx <= left + btnW && my >= saveTop && my <= saveTop + btnH) {
                inSaveBtn = 1;
            }
            drawlosinginterface(cfg.scr_w, cfg.scr_h, inRestartBtn, inQuitBtn,inSaveBtn);

            if (isClicked) {
                if (inRestartBtn) {
                    ResetGame(&player1, &cfg, &canFly, &order, &count, head, &isSpacePressed, (int*)&uiState);
                }
                else if (inQuitBtn) {
                    clear_linklist(head);
                    free(head);
                    closegraph();
                    return 0;

                }
                else if (inSaveBtn) {
                    createfile_archive(Username,&cfg, player1.data);
                    ShowToast(&myToast, _T("saved successfully"), 480, 60, 60);
                }
                else {
                    ;
                }
            }

        }
        if (uiState == 1) {// 游戏界面

            cleardevice();
            if (isLoggedIn) {
                displayID(Username);//显示id
            }
            if (count >= 120) {
                count = 0;
                append_linknode(head, 960, order);
                order++;

            }

            move_all_nodes(head, &cfg);
            delete_first_node_if(head);
            // 生成游戏画面
            DrawLinkList(head, 80);
            DrawGame(&player1, canFly);
            // 显示个人信息
            showscore(int(player1.data));
            showbestscore(int(bestScore));
            if (CheckCollision(&player1, head)) {
                uiState = UILosing;
            }
            if (search_archive(Username) > 0) bestScore = search_archive(Username);
            UpdatePhysics(&player1, &cfg, isSpacePressed, &canFly);
            UpdateEnvironment(&player1, &cfg, isSpacePressed, &canFly);
            if (player1.data > 1000000000) {
               
                uiState = UIFinish;
            }

        }
        if (uiState == UILogging) {// 登录界面
            drawlogininterface();
            ipb = 0;
            if (isEnterPressed && ipb == 0){
                ipb = InputBox(Username, 14, _T("Enter your user name in less than 12 character"), _T("username"));
                if (ipb) {
                    uiState = UIStart;
                    isLoggedIn = 1;
					ShowToast(&myToast, _T("login successful"), 480, 60, 60);

					if (search_archive(Username) > 0) bestScore = search_archive(Username);
                }
            }
            
        }
		if (uiState == UIFinish) {
            if (count > 80){
            drawfinalinterface(960, 640, player1.data);
			clear_linklist(head);//清理链表
			createfile_archive(Username, &cfg, player1.data);//finish后自动保存
            }
        }
        RenderToast(&myToast);
        FlushBatchDraw();
        count++;
        Sleep(20);
    }

}