#include <stdio.h>
#include <stdarg.h>

int sum(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int total = 0;
    for (int i = 0; i < count; i++) total += va_arg(ap, int);
    va_end(ap);
    return total;
}

double average(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double total = 0;
    for (int i = 0; i < count; i++) total += va_arg(ap, double);
    va_end(ap);
    return count ? total / count : 0;
}

int main(void) {
    printf("%d %d %d\n", sum(3, 1, 2, 3), sum(0), sum(5, 10, 20, 30, 40, 50));
    printf("%.2f\n", average(4, 1.0, 2.0, 3.0, 4.5));
    return 0;
}
