#pragma once

#include<graphics.h>

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

