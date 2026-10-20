#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int sum(const int *arr, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) s += arr[i];
    return s;
}

void fill(int *arr, int n, int v) {
    for (int *p = arr; p < arr + n; p++) *p = v;
}

int main(void) {
    int x = 10, y = 20;
    int *px = &x;
    *px += 5;
    printf("%d %d\n", x, *px);
    swap(&x, &y);
    printf("%d %d\n", x, y);
    int arr[5] = {1, 2, 3, 4, 5};
    int *p = arr;
    printf("%d %d %d\n", *(p + 2), p[4], *(arr + 1));
    printf("%d\n", sum(arr, 5));
    fill(arr, 3, 9);
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");
    int **pp = &px;
    **pp = 100;
    printf("%d\n", y);
    int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int total = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++) total += m[i][j];
    printf("%d\n", total);
    int *np = NULL;
    printf("%s\n", np == NULL ? "null" : "set");
    return 0;
}
