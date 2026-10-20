#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int apply(int (*op)(int, int), int a, int b) { return op(a, b); }

void forEach(int *arr, int n, void (*fn)(int)) {
    for (int i = 0; i < n; i++) fn(arr[i]);
}

void show(int x) { printf("[%d]", x); }

typedef int (*BinOp)(int, int);

int main(void) {
    int (*ops[3])(int, int) = {add, sub, mul};
    const char *names[] = {"add", "sub", "mul"};
    for (int i = 0; i < 3; i++) printf("%s=%d ", names[i], ops[i](7, 3));
    printf("\n%d\n", apply(mul, 6, 7));
    BinOp f = sub;
    printf("%d\n", f(10, 4));
    int arr[] = {1, 2, 3};
    forEach(arr, 3, show);
    printf("\n");
    return 0;
}
