#include <stdio.h>
template <typename T>
T twice(T x) { return x + x; }
int main(void) {
    printf("%d\n", twice(21));
    return 0;
}
