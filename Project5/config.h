#pragma once

#include<graphics.h>
typedef struct {
    int active;      // 是否激活
    int timer;       // 剩余帧数
    int x, y;        // 位置
    int duration;    // 总时长
    TCHAR text[64];  // 提示的内容
} UI_Toast;

struct linknode;
struct GlobalConfig {
    double gravity, thrust, v_term_up, v_term_down;
    double energy_max, energy_consume, energy_rec_fall, energy_rec_floor, energy_restart_p;
    int scr_w, scr_h, bnd_top, bnd_btm;
    int obstacle_speed;
};

// 声明初始化函数
void InitConfig(struct GlobalConfig* cfg);

typedef struct pos {
    double x;
    double y;

}pos;
struct circle {
    pos Pos;
    double vy;
    double energy;
    double maxenergy;
    long long data;
    int isPassing;
    struct linknode* passingWall;
};
struct box {
    int x;
    int y;
    int is_active;
    long long num;
};
typedef struct Colorpair {
    COLORREF bg;
    COLORREF text;
} ColorPair;
void ShowToast(UI_Toast* toast, const TCHAR* message, int x, int y, int frames);
