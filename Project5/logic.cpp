#include <stdlib.h>
#include "logic.h"
#include <math.h>
#include <time.h>
void create_linklist(linknode* head) {
    head->next = nullptr;
    head->prev = nullptr;
}
void append_linknode(linknode* head, int xpos, int order) {
    srand((unsigned int)time(NULL));
    if (head == NULL) return;

    linknode* newnode = (linknode*)calloc(1, sizeof(linknode));
    if (newnode == NULL) return;

    // 初始化位置和数据
    for (int i = 0; i < 8; i++) {
        newnode->Pos[i].x = xpos;
        newnode->Pos[i].y = 0 + 80 * i;
        newnode->data[i] = 0;
    }

    // 生成主值 2^order
    long long main_val = (long long)pow(2, order);
    int main_pos = rand() % 8;
    newnode->data[main_pos] = main_val;

    // 生成候选集合
#define MAX_CANDIDATES 8
    int candidates[MAX_CANDIDATES];
    int cand_count = 0;

    // 向下
    for (int i = order - 1; i >= 1 && cand_count < MAX_CANDIDATES; i--) {
        candidates[cand_count++] = i;
    }
    // 向上
    for (int i = order + 1; cand_count < MAX_CANDIDATES; i++) {
        candidates[cand_count++] = i;
    }

    // 随机选择 7 个整数放入 data
    int chosen[7];
    for (int i = 0; i < 7; i++) {
        int idx = rand() % cand_count;
        chosen[i] = candidates[idx];

        // 删除已选元素，保证不重复
        candidates[idx] = candidates[cand_count - 1];
        cand_count--;
    }

    // 放入剩余空位
    int idx = 0;
    for (int i = 0; i < 8; i++) {
        if (i == main_pos) continue;
        newnode->data[i] = (long long)pow(2, chosen[idx++]);
    }

    // 再次整体打乱 data
    for (int i = 0; i < 8; i++) {
        int j = rand() % 8;
        long long tmp = newnode->data[i];
        newnode->data[i] = newnode->data[j];
        newnode->data[j] = tmp;
    }

    // 初始化指针
    newnode->next = NULL;
    newnode->prev = NULL;

    // 插入链表末尾
    linknode* p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = newnode;
    newnode->prev = p;
}
void delete_first_node(linknode* head) {
    if (head == NULL) return;
    if (head->next == NULL) return;

    linknode* first = head->next;
    linknode* second = first->next;

    head->next = second;

    if (second != NULL) {
        second->prev = head;
    }

    free(first);
}

int delete_first_node_if(linknode* head) {
    if (head == NULL || head->next == NULL) return 0;

    linknode* p = head->next;  // 从第一个数据节点开始

    while (p != NULL) {
        //判断条件：最右边的点完全移出屏幕（可改用 Pos[0] 看你需求）
        if (p->Pos[7].x < -79) {

            // 保存前后指针（因为马上要 free(p)）
            linknode* prev = p->prev;
            linknode* next = p->next;

            // 重新链接双向链表
            if (prev != NULL) {
                prev->next = next;
            }
            if (next != NULL) {
                next->prev = prev;
            }

            // 释放内存
            free(p);

            return 1;  //成功删除一个，立即返回
        }
        p = p->next;  // 继续找下一个
    }

    return 0;  //没找到满足条件的节点
}