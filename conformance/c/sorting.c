#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(const int *a, int n) {
    for (int i = 0; i < n; i++) printf("%d%c", a[i], i + 1 < n ? ' ' : '\n');
}

void bubble(int *a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) { int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; }
}

void insertion(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; j--; }
        a[j + 1] = key;
    }
}

void merge(int *a, int l, int m, int r) {
    int tmp[64], i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int t = 0; t < k; t++) a[l + t] = tmp[t];
}

void mergeSort(int *a, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    merge(a, l, m, r);
}

int partition(int *a, int lo, int hi) {
    int pivot = a[hi], i = lo - 1;
    for (int j = lo; j < hi; j++)
        if (a[j] < pivot) { i++; int t = a[i]; a[i] = a[j]; a[j] = t; }
    int t = a[i + 1]; a[i + 1] = a[hi]; a[hi] = t;
    return i + 1;
}

void quickSort(int *a, int lo, int hi) {
    if (lo < hi) {
        int p = partition(a, lo, hi);
        quickSort(a, lo, p - 1);
        quickSort(a, p + 1, hi);
    }
}

int cmpInt(const void *x, const void *y) { return *(const int *)x - *(const int *)y; }
int cmpDesc(const void *x, const void *y) { return *(const int *)y - *(const int *)x; }

int main(void) {
    int base[] = {29, 3, 72, 15, 8, 41, 0, -6, 15};
    int n = sizeof(base) / sizeof(base[0]);
    int a[16];
    memcpy(a, base, sizeof(base)); bubble(a, n); print(a, n);
    memcpy(a, base, sizeof(base)); insertion(a, n); print(a, n);
    memcpy(a, base, sizeof(base)); mergeSort(a, 0, n - 1); print(a, n);
    memcpy(a, base, sizeof(base)); quickSort(a, 0, n - 1); print(a, n);
    memcpy(a, base, sizeof(base)); qsort(a, n, sizeof(int), cmpInt); print(a, n);
    qsort(a, n, sizeof(int), cmpDesc); print(a, n);
    return 0;
}
