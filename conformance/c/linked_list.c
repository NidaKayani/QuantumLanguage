#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *push(Node *head, int v) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->data = v;
    n->next = head;
    return n;
}

Node *append(Node *head, int v) {
    Node *n = malloc(sizeof(Node));
    n->data = v;
    n->next = NULL;
    if (!head) return n;
    Node *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = n;
    return head;
}

Node *removeValue(Node *head, int v) {
    Node *prev = NULL, *cur = head;
    while (cur && cur->data != v) { prev = cur; cur = cur->next; }
    if (!cur) return head;
    if (prev) prev->next = cur->next; else head = cur->next;
    free(cur);
    return head;
}

Node *reverse(Node *head) {
    Node *prev = NULL;
    while (head) {
        Node *next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}

void print(const Node *head) {
    for (const Node *p = head; p; p = p->next) printf("%d -> ", p->data);
    printf("NULL\n");
}

int length(const Node *head) { return head ? 1 + length(head->next) : 0; }

int main(void) {
    Node *list = NULL;
    for (int i = 1; i <= 5; i++) list = append(list, i * 10);
    list = push(list, 5);
    print(list);
    list = removeValue(list, 30);
    list = removeValue(list, 5);
    print(list);
    list = reverse(list);
    print(list);
    printf("length %d\n", length(list));
    while (list) {
        Node *t = list;
        list = list->next;
        free(t);
    }
    print(list);
    return 0;
}
