#include"config.h"
#include"physics.h"
#include <stdio.h>
void UpdatePhysics(struct circle* p, const struct GlobalConfig* cfg, int isSpacePressed, int* canFly) {
    if (p->isPassing == 1) {
        p->vy = 0;
        p->energy += cfg->energy_rec_floor;
        return;
    }
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
int CheckCollision(struct circle* player, linknode* head) {
    printf("CheckCollision called\n");
    if (head == NULL) return 0;

    int boxSize = 80;
    int player_left = (int)player->Pos.x - boxSize / 2;
    int player_right = (int)player->Pos.x + boxSize / 2;
    int player_top = (int)player->Pos.y - boxSize / 2;
    int player_bottom = (int)player->Pos.y + boxSize / 2;

    // ========== 阶段1：已经在通道中，检查是否还在 ==========
    if (player->isPassing == 1 && player->passingWall != NULL) {
        int wall_left = (int)player->passingWall->Pos[0].x;
        int wall_right = wall_left + boxSize;

        if (player_right > wall_left && player_left < wall_right) {
            // 还在这面墙里，找到对应缺口继续吸附
            for (int i = 0; i < 8; i++) {
                if (player->passingWall->data[i] == player->data) {
                    int gap_center_y = (int)player->passingWall->Pos[i].y + boxSize / 2;
                    player->Pos.y = (double)gap_center_y;
                    player->vy = 0;
                    return 0;  // 安全，继续通过
                }
            }
        }
        else {
            // 墙移走了，通过完成！
            player->isPassing = 0;
            player->passingWall = NULL;
            player->data *= 2;  // 翻倍：1→2→4→8→16...
            return 0;  // ★ 翻倍后直接返回，这帧不再检查其他墙
        }
    }

    // ========== 阶段2：检查所有墙壁，是否有新碰撞 ==========
    linknode* p = head->next;
    while (p != NULL) {
        int wall_left = (int)p->Pos[0].x;
        int wall_right = wall_left + boxSize;

        // 水平方向：玩家与墙壁是否重叠？
        if (player_right > wall_left && player_left < wall_right) {

            // 找 data 与玩家相等的缺口
            int passable_index = -1;
            for (int i = 0; i < 8; i++) {
                if (p->data[i] == player->data) {
                    passable_index = i;
                    break;
                }
            }

            // 没有匹配的缺口，直接碰撞
            if (passable_index == -1) {
                return 1;
            }

            // 缺口的位置信息
            int gap_top = (int)p->Pos[passable_index].y;
            int gap_bottom = gap_top + boxSize;
            int gap_center_y = gap_top + boxSize / 2;
            int player_center_y = (int)player->Pos.y;
            int tolerance = 10;

            // 判断能否进入通道
            if (abs(player_center_y - gap_center_y) <= tolerance) {
                // 容差范围内，吸附进入通道
                player->Pos.y = (double)gap_center_y;
                player->vy = 0;
                player->isPassing = 1;
                player->passingWall = p;
                return 0;
            }
            else if (player_top >= gap_top && player_bottom <= gap_bottom) {
                // 玩家完全在缺口内
                player->Pos.y = (double)gap_center_y;
                player->vy = 0;
                player->isPassing = 1;
                player->passingWall = p;
                return 0;
            }
            else {
                // 对不准，撞墙
                return 1;
            }
        }

        p = p->next;
    }

    return 0;  // 没有碰撞
}

