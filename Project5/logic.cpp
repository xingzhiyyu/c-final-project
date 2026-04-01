#include <stdlib.h>
#include "logic.h"
void create_linklist(linknode* head) {
    head->next = nullptr;
    head->prev = nullptr;
}
void append_linknode(linknode* head, int xpos) {
    if (head == NULL) return;

    // ✅ 1. 创建新节点并用 calloc 清零（避免垃圾指针）
    linknode* newnode = (linknode*)calloc(1, sizeof(linknode));
    if (newnode == NULL) return;  // 内存分配失败保护

    // ✅ 2. 初始化位置数据
    for (int i = 0; i < 8; i++) {
        newnode->Pos[i].x = xpos;
        newnode->Pos[i].y = 0 + 80 * i;  // 垂直排列 8 个点
    }

    // ✅ 3. 显式初始化指针（双重保险）
    newnode->next = NULL;
    newnode->prev = NULL;

    // ✅ 4. 找到链表末尾（遍历到最后一个节点）
    linknode* p = head;
    while (p->next != NULL) {
        p = p->next;
    }

    // ✅ 5. 双向链接新节点
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