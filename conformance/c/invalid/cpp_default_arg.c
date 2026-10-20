#include <stdio.h>
int scale(int x, int factor = 2) { return x * factor; }
int main(void) {
    printf("%d\n", scale(4));
    return 0;
}
