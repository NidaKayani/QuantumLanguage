#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstring>
#include <cstdio>
using namespace std;

namespace geometry {
    const double PI = 3.14159;
    double circleArea(double r) { return PI * r * r; }
    namespace detail {
        int helper() { return 7; }
    }
}

int counter() {
    static int calls = 0;
    return ++calls;
}

int globalTotal = 0;

void addGlobal(int x) { globalTotal += x; }

int sumDigits(int n) { return n < 10 ? n : n % 10 + sumDigits(n / 10); }

bool isPalindrome(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        while (i < j && !isalnum(s[i])) i++;
        while (i < j && !isalnum(s[j])) j--;
        if (tolower(s[i]) != tolower(s[j])) return false;
        i++; j--;
    }
    return true;
}

string caesar(const string& s, int k) {
    string out = s;
    for (char& c : out) {
        if (isupper(c)) c = 'A' + (c - 'A' + k) % 26;
        else if (islower(c)) c = 'a' + (c - 'a' + k) % 26;
    }
    return out;
}

int main() {
    cout << geometry::circleArea(2) << " " << geometry::detail::helper() << endl;
    counter(); counter();
    cout << counter() << endl;
    for (int i = 1; i <= 4; i++) addGlobal(i);
    cout << globalTotal << endl;
    cout << sumDigits(98765) << endl;
    cout << isPalindrome("A man, a plan, a canal: Panama") << isPalindrome("hello") << endl;
    cout << caesar("Hello, World", 3) << endl;
    int vowels = 0;
    for (char c : string("Programming in C++")) if (strchr("aeiouAEIOU", c)) vowels++;
    cout << vowels << " vowels" << endl;
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d|%.3f|%s", 7, 2.0 / 3, "ok");
    cout << buf << " " << strlen(buf) << endl;
    printf("%d %5.2f %-6s| %c %x\n", 42, 3.14159, "left", 'Q', 255);
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int t = 0;
    for (auto& row : matrix) for (int v : row) t += v;
    cout << t << endl;
    int x = 7;
    string parity = x % 2 == 0 ? "even" : "odd";
    cout << parity << endl;
    int i = 0;
    do { i += 3; } while (i < 10);
    cout << i << endl;
    for (int k = 0; k < 10; k++) {
        if (k == 2) continue;
        if (k == 5) break;
        cout << k;
    }
    cout << endl;
    switch (x) {
        case 1: cout << "one"; break;
        case 7: cout << "seven";
        case 8: cout << "-fallthrough"; break;
        default: cout << "other";
    }
    cout << endl;
    return 0;
}
