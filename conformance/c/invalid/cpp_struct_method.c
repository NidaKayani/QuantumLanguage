#include <stdio.h>
struct Counter {
    int n;
    void inc() { n++; }
};
int main(void) {
    struct Counter c = {0};
    c.inc();
    printf("%d\n", c.n);
    return 0;
}
