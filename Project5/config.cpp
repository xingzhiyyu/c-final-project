#include "config.h"
#include <tchar.h>

void InitConfig(struct GlobalConfig* cfg) {
    // 物理参数
    cfg->gravity = 0.4;
    cfg->thrust = 0.8;
    cfg->v_term_up = -12.0;
    cfg->v_term_down = 18.0;

    // 能量系统
    cfg->energy_max = 100.0;
    cfg->energy_consume = 0.24;
    cfg->energy_rec_fall = 0.24;
    cfg->energy_rec_floor = 0.12;
    cfg->energy_restart_p = 0.3;

    // 屏幕与边界
    cfg->scr_w = 960;
    cfg->scr_h = 640;
    cfg->bnd_top = 40;
    cfg->bnd_btm = 600;

    // 障碍物设定
    cfg->obstacle_speed = 6;
}
void ShowToast(UI_Toast* toast, const TCHAR* message, int x, int y, int frames) {
    if (!toast) return;

    toast->active = 1;
    toast->timer = frames;
    toast->duration = frames;
    toast->x = x;
    toast->y = y;

    // 拷贝自定义内容
    _tcscpy_s(toast->text, 64, message);
}