#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;
    int *a = malloc(n * sizeof(int));
    if (a == NULL) return 1;
    for (int i = 0; i < n; i++) a[i] = i * i;
    a = realloc(a, 2 * n * sizeof(int));
    for (int i = n; i < 2 * n; i++) a[i] = -i;
    for (int i = 0; i < 2 * n; i++) printf("%d ", a[i]);
    printf("\n");
    free(a);
    double *z = calloc(4, sizeof(double));
    printf("%.1f %.1f\n", z[0], z[3]);
    free(z);
    int rows = 3, cols = 4;
    int **grid = malloc(rows * sizeof(int *));
    for (int r = 0; r < rows; r++) {
        grid[r] = malloc(cols * sizeof(int));
        for (int c = 0; c < cols; c++) grid[r][c] = r * cols + c;
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) printf("%3d", grid[r][c]);
        printf("\n");
        free(grid[r]);
    }
    free(grid);
    char *s = malloc(6);
    for (int i = 0; i < 5; i++) s[i] = 'a' + i;
    s[5] = '\0';
    printf("%s\n", s);
    free(s);
    return 0;
}
