/*#include <graphics.h>
#include "config.h"
#include "physics.h"
#include "draw.h"
#include "config.h"
#include "logic.h"


int main() {
    int count = 0;
    struct GlobalConfig cfg;
    InitConfig(&cfg);
    int order = 0;

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
    //UI状态
    int gameStarted = 0;
    int isLoggedIn = 0;
    int logging = 0;
    int inloginbutton = 0;
    int instartbutton = 0;
    int esc_interface = 0;
    //鼠标位置
    int mx = 0;
    int my = 0;

    initgraph(cfg.scr_w, cfg.scr_h);
    BeginBatchDraw();

    ExMessage msg;

    linknode* head = (linknode*)malloc(sizeof(linknode));
    create_linklist(head);

    while (1) {
        //开始界面
        cleardevice();
        setbkcolor(RGB(131, 181, 217));
        if (esc_interface) {
            cleardevice();
            outtextxy(cfg.scr_w / 2 - textwidth(_T("Are you sure you want to quit?")) / 2, cfg.scr_h / 2 - 100, _T("Are you sure you want to quit?"));
            outtextxy(cfg.scr_w / 2 - textwidth(_T("Press ESC again to confirm, or click anywhere to cancel.")) / 2, cfg.scr_h / 2, _T("Press ESC again to confirm, or click anywhere to cancel."));
            if (peekmessage(&msg)) {
                setbkcolor(BLACK);
                settextcolor(WHITE);
                //键盘

                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                    clear_linklist(head);
                    free(head);
                    closegraph();
                    return 0;
                }
                if (msg.message == WM_KEYDOWN && msg.vkcode != VK_ESCAPE) {
                    esc_interface = 0;
                    FlushBatchDraw();
                    continue;
                }


                // 鼠标

                if (msg.message == WM_LBUTTONDOWN) {
                    esc_interface = 0;
                    FlushBatchDraw();
                    continue;
                }
            }
        }

        if (!gameStarted && !logging && !esc_interface) {

            int btnWid = 220;
            int btnHei = 60;
            int gap = 24;
            int left = cfg.scr_w / 2 - btnWid / 2;
            int startTop = cfg.scr_h / 2 - 40;
            int loginTop = startTop + btnHei + gap;


            while (peekmessage(&msg)) {

                //更新鼠标位置
                mx = msg.x;
                my = msg.y;
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                    cleardevice();
                    esc_interface = 1;
                }

                //处理逻辑
                if (msg.message == WM_LBUTTONDOWN) {

                    if (mx >= left && mx <= left + btnWid &&
                        my >= startTop && my <= startTop + btnHei) {
                        gameStarted = 1;
                    }
                    else if (mx >= left && mx <= left + btnWid &&
                        my >= loginTop && my <= loginTop + btnHei) {
                        logging = 1;
                    }
                }
            }


            instartbutton = 0;
            inloginbutton = 0;

            if (mx >= left && mx <= left + btnWid && my >= startTop && my <= startTop + btnHei) {
                instartbutton = 1;
            }

            if (mx >= left && mx <= left + btnWid && my >= loginTop && my <= loginTop + btnHei) {
                inloginbutton = 1;

            }


            DrawStartScreen(cfg.scr_w, cfg.scr_h, isLoggedIn, inloginbutton, instartbutton);
        }


        else if (!gameStarted && logging) {

            while (peekmessage(&msg, EX_KEY)) {
                if (msg.vkcode == VK_ESCAPE && msg.message == WM_KEYDOWN) {
                    logging = 0;
                }
            }
            drawlogininterface();
        }
        else if(gameStarted) {//游戏开始

            if (count >= 120) {
                count = 0;
                append_linknode(head, 960,order);
                order++;

            }
            move_all_nodes(head,&cfg);
            delete_first_node_if(head);

            // 输入
            while (peekmessage(&msg, EX_KEY)) {
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE) {
                    isSpacePressed = 1;
                }
                if (msg.message == WM_KEYUP && msg.vkcode == VK_SPACE) {
                    isSpacePressed = 0;
                }
                if (msg.vkcode == VK_ESCAPE && msg.message == WM_KEYDOWN) {
                    gameStarted = 0;
                    //游戏内部重置
                    player1.Pos.y = 320;
                    player1.vy = 0;
                    player1.energy = cfg.energy_max;
                    player1.data = 1;
                    player1.isPassing = 0;
                    player1.passingWall = NULL;
                    canFly = 1;
                    order = 0;
                    count = 0;
                    clear_linklist(head);
                    isSpacePressed = 0;
                    FlushBatchDraw();
                    continue;
                }

            }



            // 逻辑层 (Physics)
            UpdatePhysics(&player1, &cfg, isSpacePressed, &canFly);
            UpdateEnvironment(&player1, &cfg, isSpacePressed, &canFly);

            if (CheckCollision(&player1, head)) {
                // 先画游戏场景
                DrawLinkList(head, 80);
                DrawGame(&player1, canFly);

                int waiting = 1;
                int lmx = 0, lmy = 0;  // losing界面的鼠标坐标

                int btnW = 220, btnH = 60, gap = 24;
                int left = cfg.scr_w / 2 - btnW / 2;
                int restartTop = cfg.scr_h / 2 - 40;
                int quitTop = restartTop + btnH + gap;

                while (waiting) {
                    int inRestartBtn = 0;
                    int inQuitBtn = 0;

                    while (peekmessage(&msg, EX_MOUSE)) {
                        lmx = msg.x;
                        lmy = msg.y;

                        if (msg.message == WM_LBUTTONDOWN) {
                            if (lmx >= left && lmx <= left + btnW &&
                                lmy >= restartTop && lmy <= restartTop + btnH) {
                                waiting = 0;  // restart
                            }
                            else if (lmx >= left && lmx <= left + btnW &&
                                lmy >= quitTop && lmy <= quitTop + btnH) {
                                clear_linklist(head);
                                free(head);
                                closegraph();
                                return 0;  // quit
                            }
                        }
                    }

                    // 悬停检测
                    if (lmx >= left && lmx <= left + btnW && lmy >= restartTop && lmy <= restartTop + btnH) {
                        inRestartBtn = 1;
                    }
                    if (lmx >= left && lmx <= left + btnW && lmy >= quitTop && lmy <= quitTop + btnH) {
                        inQuitBtn = 1;
                    }

                    // 重绘（保留游戏背景）
                    cleardevice();
                    DrawLinkList(head, 80);
                    DrawGame(&player1, canFly);
                    drawlosinginterface(cfg.scr_w, cfg.scr_h, inRestartBtn, inQuitBtn);
                    FlushBatchDraw();
                    Sleep(16);
                }

                // 重置游戏
                player1.Pos.y = 320;
                player1.vy = 0;
                player1.energy = cfg.energy_max;
                player1.data = 1;
                player1.isPassing = 0;
                player1.passingWall = NULL;
                canFly = 1;
                order = 0;
                count = 0;
                clear_linklist(head);
                isSpacePressed = 0;
                continue;
            }

            // 视图层 (Render)
            DrawLinkList(head, 80);
            DrawGame(&player1, canFly);


        }

        FlushBatchDraw();
        count++;
        Sleep(16);
    }
    closegraph();
    return 0;
}
*/
#include <graphics.h>
#include "config.h"
#include "physics.h"
#include "draw.h"
#include "config.h"
#include "logic.h"


int main() {
    int count = 0;
    struct GlobalConfig cfg;
    InitConfig(&cfg);
    int order = 0;
    int isLoggedIn = 0;

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
    //UI状态
    enum { UIStart = 0, UIGame, UILosing, UIEsc, UILogging } uiState = UIStart;
    //鼠标位置
    int mx = 0;
    int my = 0;

    int isClicked = 0;

    initgraph(cfg.scr_w, cfg.scr_h);
    BeginBatchDraw();

    ExMessage msg;

    linknode* head = (linknode*)malloc(sizeof(linknode));
    create_linklist(head);

    while (1) {
        setbkcolor(RGB(131, 181, 217));
        BeginBatchDraw();
        cleardevice();

        while (peekmessage(&msg)) {


            if (msg.message == WM_KEYDOWN) {
                if (msg.vkcode == VK_ESCAPE) {
                    if (uiState == UIStart) uiState = UIEsc;
                    else if (uiState == UIGame) uiState = UIStart;
                }

                if (msg.vkcode == VK_SPACE) {
                    isSpacePressed = 1;
                }
            }

            else if (msg.message == WM_KEYUP) {
                if (msg.vkcode == VK_SPACE) {
                    isSpacePressed = 0;
                }
            }
            else if (msg.message == WM_MOUSEMOVE) {
                mx = msg.x;
                my = msg.y;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                mx = msg.x;
                my = msg.y;
                isClicked = 1;
            }
            else if (msg.message == WM_LBUTTONUP) {
                mx = msg.x;
                my = msg.y;
                isClicked = 0;
            }
        }
        if (uiState == 0) {
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
                else if (inloginbutton) {
                    uiState = UILogging;
                }
            }

        }
        else if (uiState == 3) {
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
        {
            cleardevice();
            DrawLinkList(head, 80);
            DrawGame(&player1, canFly);

            int btnW = 220, btnH = 60, gap = 24;
            int left = cfg.scr_w / 2 - btnW / 2;
            int restartTop = cfg.scr_h / 2 - 40;
            int quitTop = restartTop + btnH + gap;
            int inRestartBtn = 0;
            int inQuitBtn = 0;

            if (mx >= left && mx <= left + btnW && my >= restartTop && my <= restartTop + btnH) {
                inRestartBtn = 1;
            }
            if (mx >= left && mx <= left + btnW && my >= quitTop && my <= quitTop + btnH) {
                inQuitBtn = 1;
            }
            drawlosinginterface(cfg.scr_w, cfg.scr_h, inRestartBtn, inQuitBtn);

            if (isClicked) {
                if (inRestartBtn) {
                    player1.Pos.y = 320;
                    player1.vy = 0;
                    player1.energy = cfg.energy_max;
                    player1.data = 1;
                    player1.isPassing = 0;
                    player1.passingWall = NULL;
                    canFly = 1;
                    order = 0;
                    count = 0;
                    clear_linklist(head);
                    isSpacePressed = 0;
                    uiState = UIStart;
                }
                else if (inQuitBtn) {
                    clear_linklist(head);
                    free(head);
                    closegraph();
                    return 0;

                }
                else {
                    ;
                }
            }

        }
        if (uiState == 1) {
            cleardevice();
            if (count >= 120) {
                count = 0;
                append_linknode(head, 960, order);
                order++;

            }
            move_all_nodes(head, &cfg);
            delete_first_node_if(head);

            DrawLinkList(head, 80);
            DrawGame(&player1, canFly);
            if (CheckCollision(&player1, head)) {
                uiState = UILosing;
            }
            UpdatePhysics(&player1, &cfg, isSpacePressed, &canFly);
            UpdateEnvironment(&player1, &cfg, isSpacePressed, &canFly);

        }

        FlushBatchDraw();
        count++;
        Sleep(20);
    }

}