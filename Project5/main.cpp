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

    struct circle player1;
    player1.Pos.x = 40;
    player1.Pos.y = 320;
    player1.vy = 0;
    player1.maxenergy = cfg.energy_max;
    player1.energy = cfg.energy_max;
    player1.data = 1;
    player1.isPassing = 0;

    int canFly = 1, isSpacePressed = 0;
    int gameStarted = 0;
    int isLoggedIn = 0;
    int logging = 0;
    int inloginbutton = 0;
    int instartbutton = 0;
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
        if (!gameStarted && !logging) {

            int btnWid = 220;
            int btnHei = 60;
            int gap = 24;
            int left = cfg.scr_w / 2 - btnWid / 2;
            int startTop = cfg.scr_h / 2 - 40;
            int loginTop = startTop + btnHei + gap;

            while (peekmessage(&msg, EX_MOUSE)) {

                //更新鼠标位置
                mx = msg.x;
                my = msg.y;

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
            drawlogininterface();
        }
        else {//游戏开始
          
            if (count >= 120) {
                count = 0;
                append_linknode(head, 960,order);
                order++;

            }
            move_all_nodes(head,&cfg);
            delete_first_node_if(head);

            // 输入
            while (peekmessage(&msg, EX_KEY)) {
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE) isSpacePressed = 1;
                if (msg.message == WM_KEYUP && msg.vkcode == VK_SPACE) isSpacePressed = 0;
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
