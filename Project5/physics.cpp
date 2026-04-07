#include"config.h"
#include"physics.h"
void UpdatePhysics(struct circle* p, const struct GlobalConfig* cfg, int isSpacePressed, int* canFly) {
    if (isSpacePressed && *canFly) {
        // 飞行消耗
        p->energy -= cfg->energy_consume;
        p->vy -= (p->vy > 0) ? (2 * cfg->thrust - cfg->gravity) : (cfg->thrust - cfg->gravity);
    }
    else {
        p->vy += cfg->gravity;
    }

    // 速度限制
    if (p->vy > cfg->v_term_down) {
        p->vy = cfg->v_term_down;
    }
    if (p->vy < cfg->v_term_up) {
        p->vy = cfg->v_term_up;
    }

    // 位置更新
    p->Pos.y += p->vy;
}

// 边界与回能逻辑
void UpdateEnvironment(struct circle* p, const struct GlobalConfig* cfg, int isSpacePressed, int* canFly) {
    // 边界碰撞
    if (p->Pos.y <= cfg->bnd_top) {
        p->Pos.y = cfg->bnd_top; p->vy = 0;
    }
    if (p->Pos.y >= cfg->bnd_btm) {
        p->Pos.y = cfg->bnd_btm; p->vy = 0;
    }

    // 回能逻辑
    if (!isSpacePressed) {
        if (p->Pos.y >= cfg->bnd_btm) {
            p->energy += cfg->energy_rec_floor;
        }
        else if (p->vy >= 1.0) {
            p->energy += cfg->energy_rec_fall;
        }
    }
    if (p->energy < 3) p->energy += 0.05; // 基础回能

    // 能量约束
    if (p->energy > p->maxenergy) {
        p->energy = p->maxenergy;
    }
    if (p->energy < 0) {
        p->energy = 0;
    }

    // 状态机：熄火/重启
    if (p->energy <= 0) {
        *canFly = 0;
    }
    if (p->energy > p->maxenergy * cfg->energy_restart_p) {
        *canFly = 1;
    }
}
void move_all_nodes(linknode* head, const struct GlobalConfig* cfg) {
    if (head == NULL) return;

    linknode* p = head->next;  // 跳过 head（一般是哨兵）

    while (p != NULL) {

        // 每个节点有8个点
        for (int i = 0; i < 8; i++) {
            p->Pos[i].x -= cfg->obstacle_speed;
        }

        p = p->next;
    }
}