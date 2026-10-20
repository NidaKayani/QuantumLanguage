#include <stdio.h>

long long factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

double power(double base, int e) {
    if (e == 0) return 1;
    double half = power(base, e / 2);
    return e % 2 ? half * half * base : half * half;
}

void hanoi(int n, char from, char to, char via, int *moves) {
    if (n == 0) return;
    hanoi(n - 1, from, via, to, moves);
    (*moves)++;
    hanoi(n - 1, via, to, from, moves);
}

int binarySearch(const int *a, int lo, int hi, int key) {
    if (lo > hi) return -1;
    int mid = (lo + hi) / 2;
    if (a[mid] == key) return mid;
    return a[mid] < key ? binarySearch(a, mid + 1, hi, key) : binarySearch(a, lo, mid - 1, key);
}

int main(void) {
    printf("%lld %lld\n", factorial(10), factorial(20));
    for (int i = 0; i < 10; i++) printf("%d ", fib(i));
    printf("\n%d %d\n", gcd(84, 36), gcd(17, 5));
    printf("%.0f %.4f\n", power(2, 20), power(1.5, 3));
    int moves = 0;
    hanoi(6, 'A', 'C', 'B', &moves);
    printf("%d\n", moves);
    int a[] = {1, 3, 5, 7, 9, 11, 13};
    printf("%d %d\n", binarySearch(a, 0, 6, 11), binarySearch(a, 0, 6, 4));
    return 0;
}
