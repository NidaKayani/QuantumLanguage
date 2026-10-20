#include <stdio.h>
namespace util {
    int twice(int x) { return 2 * x; }
}
int main(void) {
    printf("%d\n", util::twice(4));
    return 0;
}
