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
            move_all_nodes(head);
            delete_first_node_if(head);

            // 输入
            while (peekmessage(&msg, EX_KEY)) {
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_SPACE) isSpacePressed = 1;
                if (msg.message == WM_KEYUP && msg.vkcode == VK_SPACE) isSpacePressed = 0;
            }

            

            // 逻辑层 (Physics)
            UpdatePhysics(&player1, &cfg, isSpacePressed, &canFly);
            UpdateEnvironment(&player1, &cfg, isSpacePressed, &canFly);

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
