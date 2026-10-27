#include <stdio.h>

int main(void) {
    bool ready = true;
    int *p = nullptr;
    constexpr int size = 4;
    int data[size] = {};
    for (int i = 0; i < size; i++) data[i] = i * i;
    printf("%d %s %d\n", ready, p == nullptr ? "null" : "set", data[3]);
    auto total = 0;
    for (int i = 0; i < size; i++) total += data[i];
    printf("%d\n", total);
    return 0;
}
