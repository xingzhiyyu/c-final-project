#include <stdlib.h>
#include "logic.h"
void create_linklist(linknode* head) {
    head->next = nullptr;
    head->prev = nullptr;
}
void append_linknode(linknode* head, int xpos) {
    if (head == NULL) return;

    //  创建新节点并用 calloc 清零（避免垃圾指针）
    linknode* newnode = (linknode*)calloc(1, sizeof(linknode));
    if (newnode == NULL) return;  // 内存分配失败保护

    // 初始化位置数据
    for (int i = 0; i < 8; i++) {
        newnode->Pos[i].x = xpos;
        newnode->Pos[i].y = 0 + 80 * i;  // 垂直排列 8 个点
    }

    //  初始化指针=
    newnode->next = NULL;
    newnode->prev = NULL;

    // 找到链表末尾（遍历到最后一个节点）
    linknode* p = head;
    while (p->next != NULL) {
        p = p->next;
    }

    //双向链接新节点
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