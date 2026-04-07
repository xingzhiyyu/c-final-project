#pragma once
#include "config.h"
#include "logic.h"

void UpdatePhysics(struct circle* p, const struct GlobalConfig* cfg, int isSpacePressed, int* canFly);
void UpdateEnvironment(struct circle* p, const struct GlobalConfig* cfg, int isSpacePressed, int* canFly);
void move_all_nodes(linknode* head, const struct GlobalConfig *cfg);
int CheckCollision(struct circle* player, linknode* head);