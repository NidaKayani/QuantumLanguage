#include <stdio.h>

#define N 3

void multiply(int a[N][N], int b[N][N], int c[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            c[i][j] = 0;
            for (int k = 0; k < N; k++) c[i][j] += a[i][k] * b[k][j];
        }
}

void transpose(int m[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++) {
            int t = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = t;
        }
}

void print(int m[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) printf("%4d", m[i][j]);
        printf("\n");
    }
}

int main(void) {
    int a[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int b[N][N] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int c[N][N];
    multiply(a, b, c);
    print(c);
    transpose(a);
    print(a);
    int trace = 0;
    for (int i = 0; i < N; i++) trace += c[i][i];
    printf("trace=%d\n", trace);
    return 0;
}
