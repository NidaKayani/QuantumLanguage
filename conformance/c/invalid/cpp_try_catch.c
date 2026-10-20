#include <stdio.h>
int main(void) {
    try {
        throw 1;
    } catch (int e) {
        printf("%d\n", e);
    }
    return 0;
}
