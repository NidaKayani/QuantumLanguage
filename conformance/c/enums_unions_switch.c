#include <stdio.h>

enum Color { RED, GREEN = 5, BLUE };
typedef enum { MON = 1, TUE, WED } Day;

union Value {
    int i;
    float f;
    char c;
};

const char *colorName(enum Color c) {
    switch (c) {
        case RED: return "red";
        case GREEN: return "green";
        case BLUE: return "blue";
    }
    return "?";
}

int main(void) {
    enum Color c = BLUE;
    printf("%d %d %d %s\n", RED, GREEN, BLUE, colorName(c));
    Day d = WED;
    printf("%d\n", d);
    union Value v;
    v.i = 65;
    printf("%d %c\n", v.i, v.c);
    printf("%zu\n", sizeof(union Value));
    for (int i = 0; i < 6; i++) {
        switch (i % 4) {
            case 0: printf("zero "); break;
            case 1:
            case 2: printf("small "); break;
            default: printf("other ");
        }
    }
    printf("\n");
    int grade = 85;
    char letter = grade >= 90 ? 'A' : grade >= 80 ? 'B' : 'C';
    printf("%c\n", letter);
    return 0;
}
