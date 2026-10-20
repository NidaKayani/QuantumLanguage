#include <stdio.h>
int area(int s) { return s * s; }
int area(int w, int h) { return w * h; }
int main(void) {
    printf("%d %d\n", area(3), area(2, 5));
    return 0;
}
