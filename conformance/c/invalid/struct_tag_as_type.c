#include <stdio.h>
struct Point { int x, y; };
int main(void) {
    Point p = {1, 2};
    printf("%d\n", p.x);
    return 0;
}
