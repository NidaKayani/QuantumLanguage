#include <stdio.h>
#include <stdbool.h>

#define CAP 8

typedef struct {
    int items[CAP];
    int top;
} Stack;

void push(Stack *s, int v) { if (s->top < CAP) s->items[s->top++] = v; }
int pop(Stack *s) { return s->top > 0 ? s->items[--s->top] : -1; }
bool isEmpty(const Stack *s) { return s->top == 0; }

typedef struct {
    int data[CAP];
    int head, tail, size;
} Queue;

bool enqueue(Queue *q, int v) {
    if (q->size == CAP) return false;
    q->data[q->tail] = v;
    q->tail = (q->tail + 1) % CAP;
    q->size++;
    return true;
}

int dequeue(Queue *q) {
    int v = q->data[q->head];
    q->head = (q->head + 1) % CAP;
    q->size--;
    return v;
}

int main(void) {
    Stack s = {.top = 0};
    for (int i = 1; i <= 5; i++) push(&s, i * 11);
    while (!isEmpty(&s)) printf("%d ", pop(&s));
    printf("\n");
    Queue q = {0};
    for (int i = 0; i < 10; i++)
        if (!enqueue(&q, i)) printf("full at %d\n", i);
    printf("%d %d\n", dequeue(&q), dequeue(&q));
    enqueue(&q, 100);
    while (q.size) printf("%d ", dequeue(&q));
    printf("\n");
    return 0;
}
