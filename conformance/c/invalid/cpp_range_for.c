#include <stdio.h>
int main(void) {
    int a[3] = {1, 2, 3};
    for (int x : a) printf("%d", x);
    return 0;
}
