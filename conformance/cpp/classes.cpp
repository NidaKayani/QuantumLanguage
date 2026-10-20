#include <iostream>
#include <string>
using namespace std;

class Point {
private:
    int x, y;
    static int count;
public:
    Point() : x(0), y(0) { count++; }
    Point(int x, int y) : x(x), y(y) { count++; }
    Point(const Point& o) : x(o.x), y(o.y) { count++; cout << "copy" << endl; }
    ~Point() { count--; }
    int getX() const { return x; }
    int getY() const { return y; }
    void move(int dx, int dy) { x += dx; y += dy; }
    Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
    static int getCount() { return count; }
};
int Point::count = 0;

class Counter {
    int value = 0;
public:
    Counter& inc() { value++; return *this; }
    int get() const { return value; }
};

int main() {
    Point a(1, 2), b(3, 4);
    Point c = a + b;
    cout << c << " " << Point::getCount() << endl;
    c.move(10, -1);
    cout << c.getX() << "," << c.getY() << endl;
    cout << (a == Point(1, 2)) << " " << (a == b) << endl;
    Point d = a;
    cout << d << " " << Point::getCount() << endl;
    {
        Point temp;
        cout << "inside " << Point::getCount() << endl;
    }
    cout << "after " << Point::getCount() << endl;
    Counter k;
    k.inc().inc().inc();
    cout << k.get() << endl;
    return 0;
}
