#include <stdio.h>
#include <limits.h>

int main(void) {
    int a = 17, b = 5;
    printf("%d %d %d %d\n", a / b, a % b, -a / b, -a % b);
    double x = a / 2, y = a / 2.0;
    printf("%.1f %.2f %f\n", x, y, (double)a / b);
    printf("%d %d\n", (int)3.99, (int)-3.99);
    long long big = (long long)INT_MAX * 2;
    printf("%lld %d %d\n", big, INT_MAX, INT_MIN);
    unsigned int u = 0;
    u--;
    printf("%u %x %X %o\n", u, 255, 255, 8);
    printf("%5d|%-5d|%05d|%+d\n", 42, 42, 42, 42);
    printf("%e %.3e %g %g %g\n", 12345.678, 0.000123, 100.0, 1e10, 0.0001);
    printf("%c%c%c %s %%\n", 'a', 'b' + 1, 100, "str");
    printf("%10.3f|%-10.2f|\n", 3.14159, 2.5);
    char c = 'A';
    c += 2;
    printf("%c %d\n", c, c);
    int i = 5;
    i += 3; i *= 2; i -= 1; i /= 3; i %= 4;
    printf("%d\n", i);
    printf("%d %d %d %d %d\n", 7 & 3, 7 | 8, 7 ^ 2, 1 << 4, -16 >> 2);
    printf("%zu %zu %zu\n", sizeof(char), sizeof(int), sizeof(double));
    return 0;
}
