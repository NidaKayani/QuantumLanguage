#include <stdio.h>
int main(void) {
    auto f = [](int x) { return x * 2; };
    printf("%d\n", f(3));
    return 0;
}
