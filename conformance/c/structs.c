#include <stdio.h>
#include <string.h>

struct Point {
    int x, y;
};

typedef struct {
    char name[32];
    int age;
    double gpa;
} Student;

typedef struct Rect {
    struct Point topLeft;
    int w, h;
} Rect;

int area(Rect r) { return r.w * r.h; }

void grow(Rect *r, int d) {
    r->w += d;
    r->h += d;
}

void birthday(Student *s) { s->age++; }

int main(void) {
    struct Point p = {3, 4};
    struct Point q;
    q.x = p.x * 2;
    q.y = p.y + 1;
    printf("(%d,%d) (%d,%d)\n", p.x, p.y, q.x, q.y);
    Rect r = {{0, 0}, 5, 3};
    printf("%d\n", area(r));
    grow(&r, 2);
    printf("%d %d %d\n", r.w, r.h, area(r));
    Student class[3] = {{"Ann", 20, 3.5}, {"Ben", 22, 3.1}, {"Cid", 21, 3.9}};
    birthday(&class[1]);
    double best = 0;
    int bi = 0;
    for (int i = 0; i < 3; i++) {
        printf("%s %d %.2f\n", class[i].name, class[i].age, class[i].gpa);
        if (class[i].gpa > best) { best = class[i].gpa; bi = i; }
    }
    printf("best: %s\n", class[bi].name);
    Student copy = class[0];
    strcpy(copy.name, "Zed");
    printf("%s %s\n", class[0].name, copy.name);
    Student *ps = &class[2];
    printf("%s is %d\n", ps->name, (*ps).age);
    return 0;
}
