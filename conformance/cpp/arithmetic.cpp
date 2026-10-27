#include <iostream>
#include <iomanip>
#include <climits>
#include <cmath>
using namespace std;

int main() {
    int a = 17, b = 5;
    cout << a / b << " " << a % b << " " << -a / b << " " << -a % b << endl;
    double x = a / 2, y = a / 2.0;
    cout << x << " " << y << endl;
    cout << (double)a / b << " " << static_cast<int>(3.99) << " " << (int)-3.99 << endl;
    long long big = 1LL * INT_MAX * 2;
    cout << big << " " << INT_MAX << " " << INT_MIN << " " << LLONG_MAX << endl;
    unsigned int u = 0;
    u = u - 1;
    cout << u << endl;
    cout << fixed << setprecision(3) << 3.14159265 << " " << 2.0 / 3 << endl;
    cout << scientific << 12345.678 << endl;
    cout.unsetf(ios::floatfield);
    cout << setprecision(6) << 1.0 / 3 << " " << 100.0 << " " << 1e10 << " " << 0.0001 << endl;
    cout << pow(2, 10) << " " << sqrt(144.0) << " " << abs(-7) << " " << fabs(-2.5) << endl;
    cout << floor(2.7) << " " << ceil(2.1) << " " << round(2.5) << " " << round(-2.5) << endl;
    int i = 5;
    i++;
    int j = i * 2 + ++i;
    cout << i << " " << j << endl;
    cout << (7 & 3) << " " << (7 | 8) << " " << (7 ^ 2) << " " << (1 << 4) << " " << (-16 >> 2) << endl;
    char c = 'A';
    c += 2;
    cout << c << " " << (int)c << " " << c + 1 << " " << char(c + 1) << endl;
    bool flag = 10 > 3;
    cout << flag << " " << boolalpha << flag << " " << !flag << endl;
    return 0;
}
