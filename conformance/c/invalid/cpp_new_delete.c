#include <stdio.h>
int main(void) {
    int *p = new int[5];
    delete[] p;
    return 0;
}
