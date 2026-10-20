#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);
    int sum = 0, mx = -1000000;
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        sum += x;
        if (x > mx) mx = x;
    }
    printf("sum=%d max=%d avg=%.2f\n", sum, mx, (double)sum / n);
    char name[32];
    double weight;
    scanf("%s %lf", name, &weight);
    printf("%s weighs %.1f\n", name, weight);
    char ch;
    scanf(" %c", &ch);
    printf("char=%c\n", ch);
    return 0;
}
