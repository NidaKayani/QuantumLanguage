#include <stdio.h>

#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define DEBUG 1

static int counter(void) {
    static int calls = 0;
    return ++calls;
}

int global = 10;

void bump(void) { global += 5; }

int main(void) {
    printf("%.2f %d %d\n", PI * SQUARE(2), SQUARE(3 + 1), MAX(4, 9));
#if DEBUG
    printf("debug on\n");
#else
    printf("debug off\n");
#endif
#ifdef UNDEFINED_THING
    printf("never\n");
#endif
    counter();
    counter();
    printf("%d\n", counter());
    bump();
    bump();
    printf("%d\n", global);
    const int limit = 3;
    for (int i = 0; i < limit; i++) printf("%d", i);
    printf("\n");
    return 0;
}
